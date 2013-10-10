#include "yocto/memory/arena.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/code/bswap.hpp"
#include "yocto/exceptions.hpp"

#include "yocto/memory/chunk.hpp"

#include <cerrno>
#include <cstring>


namespace yocto
{
	
	namespace memory
	{
		
		static inline size_t arena_new_maxi( size_t maxi )
		{
			const size_t new_maxi = max_of<size_t>( maxi << 1, arena::initial_chunks );
			if( new_maxi <= maxi )
				throw libc::exception( ERANGE, "arena::new_maxi overflow@%u", unsigned(maxi) );
			return new_maxi;
		}
		
		void *arena:: acquire_block()
		{
			if( accessible_ > 0 )
			{
				//==============================================================
				//
				// there is one free block somewhere
				//
				//==============================================================
				assert( pAcquiring_ );
				if( pAcquiring_->stillAvailable <= 0 )
				{
					//----------------------------------------------------------
					// local search
					//----------------------------------------------------------
					chunk       *lo           = pAcquiring_;
					chunk       *hi           = pAcquiring_;
					const chunk *hi_first_bad = chunk_last_;
					const chunk *lo_last_good = chunk_;
					
					//----------------------------------------------------------
					// interleaved search
					//----------------------------------------------------------
				SCAN_BOTH:
					if( --lo < lo_last_good )
						goto SCAN_HI_ONLY;
					else
					{
						if( lo->stillAvailable )
						{
							pAcquiring_ = lo;
							goto ACQUIRE;
						}
					}
					
					if( ++hi >= hi_first_bad )
						goto SCAN_LO_ONLY;
					else {
						if( hi->stillAvailable )
						{
							pAcquiring_ = hi;
							goto ACQUIRE;
						}
					}					
					goto SCAN_BOTH;
					
					//----------------------------------------------------------
					// lower search only
					//----------------------------------------------------------
				SCAN_LO_ONLY:
					--lo;
					assert( lo >= lo_last_good );
					if( lo->stillAvailable )
					{
						pAcquiring_ = lo;
						goto ACQUIRE;
					}
					goto SCAN_LO_ONLY;
					
					//----------------------------------------------------------
					// upper search only
					//----------------------------------------------------------
				SCAN_HI_ONLY:
					++hi;
					assert( hi < hi_first_bad );
					if( hi->stillAvailable )
					{
						pAcquiring_ = hi;
						goto ACQUIRE;
					}
					goto SCAN_HI_ONLY;					
					
				}
				
			ACQUIRE:
				assert( pAcquiring_ != NULL );
				assert( pAcquiring_->stillAvailable > 0 );
				
				//--------------------------------------------------------------
				//-- update total #blocks
				//--------------------------------------------------------------
				--accessible_;
				
				//--------------------------------------------------------------
				//-- check available cache
				//--------------------------------------------------------------
				if( pAcquiring_ == pAvailable_ )
					pAvailable_ = NULL;
				
				//--------------------------------------------------------------
				// done 
				//--------------------------------------------------------------
				return pAcquiring_->acquire(block_size);
				
			}
			else 
			{
				//==============================================================
				//
				// we need some other memory
				//
				//==============================================================
				assert( NULL == pAvailable_ ); // necessarily...
				
				
				if( size_ >= maxi_ )
				{
					//----------------------------------------------------------
					// time to increase the array of chunks
					//----------------------------------------------------------
					size_t new_maxi  = arena_new_maxi( maxi_ );                     assert( new_maxi > maxi_ );
					chunk *new_addr  = kind<global>::acquire_as<chunk>( new_maxi ); assert( new_maxi > maxi_ );
					
					memcpy( new_addr, chunk_, size_ * sizeof(chunk) );
					
					//----------------------------------------------------------
					// keep the cache status in case of memory failure
					//----------------------------------------------------------
					if( pReleasing_ )
					{
						assert( chunk_ != NULL );
						pReleasing_ = new_addr + static_cast<ptrdiff_t>( pReleasing_ - chunk_ );
					}
					
					if( pAcquiring_ )
					{
						assert( chunk_ != NULL );
						pAcquiring_ = new_addr + static_cast<ptrdiff_t>( pAcquiring_ - chunk_ );
					}
					
					//----------------------------------------------------------
					// update the array
					//----------------------------------------------------------
					kind<global>::release_as<chunk>( chunk_, maxi_ );
					
					chunk_      = new_addr;
					maxi_       = new_maxi;
					chunk_last_ = chunk_ + size_;
				}
				
				//==============================================================
				//
				// we still need some other memory
				//
				//==============================================================
				assert( size_ < maxi_ ); assert( chunk_ != NULL );
				
				//--------------------------------------------------------------
				// cretate data
				//--------------------------------------------------------------
				size_t csz = chunk_size;
				void  *ptr = kind<global>::acquire( csz ); assert( csz >= chunk_size ); assert(ptr!=NULL);
				
				//--------------------------------------------------------------
				// Append the new chunk
				//--------------------------------------------------------------
				assert( chunk_last_ == chunk_ + size_ );
				pAcquiring_  = new ( &chunk_[size_++]) chunk( ptr, block_size, num_blocks, chunk_size );
				++chunk_last_;
				
				//--------------------------------------------------------------
				// order the chunks
				//--------------------------------------------------------------
				chunk *prev = pAcquiring_;
				while( --prev >= chunk_ && prev->data >= pAcquiring_->data )
				{
					assert( prev->data > pAcquiring_->data );
					bswap<chunk>( *prev, *pAcquiring_ );
					pAcquiring_ = prev;
				}
				
				//--------------------------------------------------------------
				// update the releasing cache
				//--------------------------------------------------------------
				pReleasing_ = pAcquiring_;
				
				//--------------------------------------------------------------
				// update total number of blocks (num_blocks-1)
				// and return a valid block
				//--------------------------------------------------------------
				accessible_ += new_blocks_; 

				return pAcquiring_->acquire( block_size );
			}
			
		}
		
	}
	
}
