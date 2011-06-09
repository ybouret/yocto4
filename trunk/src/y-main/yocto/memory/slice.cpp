#include "yocto/memory/slice.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/code/ilog2.hpp"
#include "yocto/code/round.hpp"
#include "yocto/memory/global.hpp"

#if !defined(NDEBUG)
#include <iostream>
#endif

namespace yocto 
{
	namespace memory 
	{
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
		
		
#define YOCTO_MEMORY_SLICE_BLOCKS_ROUND_LN2 2
#define YOCTO_MEMORY_SLICE_BLOCKS_AVAILABLE 2
		
		static inline size_t slice_count( size_t length ) throw()
		{
			//-- entry + watch + available blocks
			const size_t raw_count = 2 + max_of<size_t>(YOCTO_MEMORY_SLICE_BLOCKS_AVAILABLE, slice::blocks_for(length));
			return YOCTO_ROUND( YOCTO_MEMORY_SLICE_BLOCKS_ROUND_LN2, raw_count );	
		}
		
		slice:: slice( size_t data_size, size_t mini_size ) :
		count_( slice_count( max_of(data_size,mini_size)  ) ),
		entry_( kind<global>::acquire_as<block_t>( count_ ) ),
		watch_( entry_+count_ )
		{
			assert( is_a_power_of_two( sizeof(block_t) ) );
			
			//------------------------------------------------------------------
			//-- initial formatting 
			//------------------------------------------------------------------
			--watch_;
			
			entry_->prev.block = NULL;        //-- no block before
			entry_->next.block = watch_;      //-- end of slice 
			entry_->size.value = count_ - 2;  //-- #blocks - (entry+watch)
			entry_->info.owner = NULL;        //-- not allocated
			
			watch_->prev.block = entry_;
			watch_->next.block = NULL;
			watch_->size.value = 0;
			watch_->info.owner = NULL;
			
			assert(entry_->size.value * sizeof(block_t) >= max_of(data_size,mini_size) );
			
		}
		
		bool slice:: is_empty() const throw()
		{
			return entry_->info.owner == NULL && entry_->next.block == watch_;
		}
		
	}
	
}
