#include "yocto/memory/slice.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/code/ilog2.hpp"
#include "yocto/code/round.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/exceptions.hpp"

#include <cstring>

#if !defined(NDEBUG)
#include <iostream>
#endif

namespace yocto 
{
	namespace memory 
	{
		size_t slice:: count() const throw()
		{
			return count_;
		}
		
		size_t slice:: bytes() const throw()
		{
			return count_ * sizeof(block_t);
		}
		
		size_t slice:: blocks_for( size_t bytes ) throw()
		{
			return (YOCTO_ROUND( ilog2<sizeof(block_t)>::value, bytes)) / sizeof(block_t);
		}
		
		slice:: ~slice() throw()
		{
#if !defined(NDEBUG)
			if( !is_empty() ) std::cerr << "[memory::slice] not empty!" << std::endl;
#endif
			kind<global>::release_as<block_t>( entry_, count_ );
			assert( NULL == entry_ ); assert( 0 == count_ );
			watch_ = NULL;
		}
		
		
		//======================================================================
		//
		// Initializing
		//
		//======================================================================
#define YOCTO_MEMORY_SLICE_BLOCKS_AVAILABLE 2
		
		static inline size_t slice_count( size_t length )
		{
			//-- entry + watch + available blocks
			const size_t raw_count = 2 + max_of<size_t>(YOCTO_MEMORY_SLICE_BLOCKS_AVAILABLE, slice::blocks_for(length));
			const size_t count     = next_power_of_two( raw_count );
			if( count < raw_count )
				throw imported::exception("memory::slice::count overflow", "too many #blocks=%u", unsigned(raw_count) );
			return count;
		}
		
		slice:: slice( size_t data_size, size_t mini_size ) :
		count_( slice_count( max_of(data_size,mini_size)  ) ),
		entry_( kind<global>::acquire_as<block_t>( count_ ) ),
		watch_( entry_+count_ ),
		ready_( 1 ),
		next(NULL),
		prev(NULL)
		{
			assert( is_a_power_of_two( sizeof(block_t) ) );
			
			//------------------------------------------------------------------
			//-- initial formatting 
			//------------------------------------------------------------------
			--watch_;
			
			assert( count_ > 2 );
			entry_->prev.block = NULL;        //-- no block before
			entry_->next.block = watch_;      //-- end of slice 
			entry_->size.value = count_ - 2;  //-- #blocks - (entry+watch)
			entry_->info.owner = NULL;        //-- not allocated
			
			watch_->prev.block = entry_;
			watch_->next.block = NULL;
			watch_->size.value = 0;
			watch_->info.owner = NULL;
			
			assert(entry_->size.value * sizeof(block_t) >= max_of(data_size,mini_size) );			
			assert( static_cast<size_t>(watch_ - entry_) == entry_->size.value+1 );
		}
		
		bool slice:: is_empty() const throw()
		{
			const bool empty_flag =  (entry_->info.owner == NULL) && (entry_->next.block == watch_);
			assert( !(empty_flag && ready_ != 1 ) );
			return empty_flag;
		}
		
		
		//======================================================================
		//
		// best fit acquiring with splitting
		//
		//======================================================================
		void * slice:: acquire( size_t &bytes ) throw()
		{
			assert( bytes > 0 );
			if( ready_ > 0 )
			{
				const size_t   value = blocks_for(bytes); assert(value>0);
				block_t       *best_block = NULL;
				size_t         best_value = 0;
				block_t       *scan = entry_;
				const block_t *last = watch_;
				size_t         scan_value = 0;
				
				//--------------------------------------------------------------
				// Find first free block with enough space
				//--------------------------------------------------------------
			FIND_FIRST:
				if( scan->info.owner == NULL && (scan_value = scan->size.value ) >= value )
				{
					best_block = scan;
					best_value = scan_value;
					goto FIND_BEST;
				}
				if( last == ( scan = scan->next.block ) )
					return NULL; //-- no block
				goto FIND_FIRST;
				
				
			FIND_BEST:
				//--------------------------------------------------------------
				// Find best block
				//--------------------------------------------------------------
				assert( scan == best_block );
				while( last != ( ( scan = scan->next.block ) ) )
				{
					if( scan->info.owner == NULL                 && 
					   (scan_value = scan->size.value ) >= value &&
					   scan_value < best_value )
					{
						best_block = scan;
						best_value = scan_value;
					}
				}
				
				//--------------------------------------------------------------
				// Found it: detect if splitting
				//--------------------------------------------------------------
				if( best_value >= 2 + value )
				{
					//std::cerr << "Should Split !" << std::endl;
					block_t *next_block = best_block->next.block; assert( NULL != next_block);
					block_t *curr_block = best_block + value + 1;
					
					//-- relinking
					next_block->prev.block = curr_block;
					curr_block->next.block = next_block;
					
					best_block->next.block = curr_block;
					curr_block->prev.block = best_block;
					
					//-- initialize
					curr_block->info.owner = NULL; //-- free
					curr_block->size.value = (best_value - value) - 1;
					
					//-- update
					++ready_;
					best_block->size.value = best_value = value;
					
					assert( static_cast<size_t>(next_block - curr_block) == curr_block->size.value+1 );
					assert( static_cast<size_t>(curr_block - best_block) == best_block->size.value+1 );
				}
				
				//--------------------------------------------------------------
				// format the block and return a clean memory area.
				//--------------------------------------------------------------
				{
					assert( best_value == best_block->size.value  );
					assert( best_value * sizeof(block_t) >= bytes );
					void *p = best_block+1;                  //-- memory address
					bytes   = best_value * sizeof(block_t ); //-- memory size
					best_block->info.owner = this;           //-- not available
					--ready_;                                //-- update cache
					memset(p,0,bytes);
					return p;
				}
				
			}
			else
			{
				return NULL;
			}
		}
		
		
		
		//======================================================================
		//
		// Releasing
		//
		//======================================================================
		
#define YMSB_FUSION_NONE  0x00
#define YMSB_FUSION_LEFT  0x01
#define YMSB_FUSION_RIGHT 0x02
#define YMSB_FUSION_BOTH  (YMSB_FUSION_LEFT|YMSB_FUSION_RIGHT)
		
		slice *slice:: release( void * p ) throw()
		{
			assert(p);
			block_t *curr_block = static_cast<block_t*>(p) - 1;
			slice   *from = curr_block->info.owner;
			assert( from != NULL );
			assert( from->entry_ != NULL );
			assert( from->watch_ != NULL );
			assert( curr_block >= from->entry_ );
			assert( curr_block <  from->watch_ );
			//std::cerr << "relasing a block of " << curr->size.value << " blk" << std::endl;
			
			block_t *prev_block = curr_block->prev.block;
			block_t *next_block = curr_block->next.block;
			
			size_t fusion = (prev_block != NULL) && (prev_block->info.owner == NULL) ? YMSB_FUSION_LEFT : YMSB_FUSION_NONE;
			if( next_block != from->watch_ && next_block->info.owner == NULL )
			{
				fusion |= YMSB_FUSION_RIGHT;
			}
			
			//std::cerr << "fusion=" << fusion << std::endl;
			switch( fusion )
			{
				case YMSB_FUSION_RIGHT: {
					block_t *after = next_block->next.block;
					assert(next_block!=from->watch_);
					assert(next_block->info.owner == NULL);
					assert( after != NULL );
					after->prev.block = curr_block;
					curr_block->next.block  = after;
					curr_block->info.owner  = NULL;
					curr_block->size.value += next_block->size.value + 1;
					assert( static_cast<size_t>(curr_block->next.block - curr_block ) == curr_block->size.value + 1 );
				} break;
					
				case YMSB_FUSION_LEFT: {
					assert(prev_block!=NULL);
					assert(prev_block->info.owner == NULL );
					prev_block->next.block = next_block;
					next_block->prev.block = prev_block;
					prev_block->size.value += 1 + curr_block->size.value;
					assert( static_cast<size_t>(prev_block->next.block - prev_block ) == prev_block->size.value + 1 );
				} break;
					
				case YMSB_FUSION_BOTH:
				{			
					block_t *after = next_block->next.block;
					assert(prev_block!=NULL);
					assert(prev_block->info.owner == NULL );
					assert(next_block!=from->watch_);
					assert(next_block->info.owner == NULL);
					assert( after != NULL );
					prev_block->next.block  = after;
					after->prev.block = prev_block;
					prev_block->size.value += 2 + curr_block->size.value + next_block->size.value;
					assert( static_cast<size_t>(prev_block->next.block - prev_block ) == prev_block->size.value + 1 );
					from->ready_--;
				} break;
					
				default:
					assert( YMSB_FUSION_NONE == fusion );
					curr_block->info.owner = NULL;
					from->ready_++;
					break;
			}
			return from;
		}
		
	}
}
