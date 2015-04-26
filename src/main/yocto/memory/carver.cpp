#include "yocto/memory/carver.hpp"
#include "yocto/memory/slice.hpp"
#include "yocto/object.hpp"
#include "yocto/code/bswap.hpp"

#include <iostream>

namespace yocto
{
	
	namespace memory
	{
		
		//======================================================================
		//
		// carver memory and setup
		//
		//======================================================================
		static inline slice *acquire_slice()
		{
			return object::acquire1<slice>();
		}
		
		static inline void   release_slice( slice *s ) throw()
		{
			assert( NULL != s );
			object::release1<slice>(s);
		}
		
		static inline void destroy_slice( slice *s ) throw()
		{
			assert( NULL != s );
			destruct(s);
			release_slice(s);
		}
		
		carver:: ~carver() throw()
		{
			list_.delete_with( destroy_slice );
			pool_.delete_with( release_slice );
		}
		
		carver:: carver( size_t mini, size_t max_unused ) throw() : 
		list_(),
		pool_(),
		free_(NULL),
		mini_size( mini ),
		max_in_pool( max_unused )
		{
			
		}
		
		//======================================================================
		//
		// memory acquire
		//
		//======================================================================
		void *carver:: acquire( size_t &bytes )
		{
			if( bytes > 0 )
			{
				//--------------------------------------------------------------
				// forward scan, auto-caching
				//-------------------------------------------------------------
				for( slice *scan = list_.head; scan != NULL; scan = scan->next )
				{
					void *p = scan->acquire( bytes );
					if( p )
					{
						
						list_.move_to_front( scan );
						if( scan == free_ ) free_ = NULL;
						return p;
					}
				}
				
				//-- free_ can be not NULL be without enough space
				
				//--------------------------------------------------------------
				//  need a new slice !
				//--------------------------------------------------------------
				slice *s = (pool_.size > 0) ? pool_.query() : acquire_slice();
				try 
				{
					new (s) slice( bytes, mini_size );					
				}
				catch (...) {
					pool_.store( s );
					throw;
				}
				list_.push_front(s);
				return s->acquire(bytes);
			}
			else
			{
				return NULL;
			}
		}
		
		//======================================================================
		//
		// memory release
		//
		//======================================================================
		void carver:: release( void *p ) throw()
		{
			if( p != NULL)
			{
				slice *s = slice::release(p);
				assert( list_.owns(s) );
				if( s->is_empty() )
				{
					if( free_ )
					{
						assert( free_->is_empty() );
						if( free_->entry_ < s->entry_ )
						{
							cswap<slice*>( free_, s);
						}
						assert( free_->entry_ > s->entry_);
						destruct( list_.unlink(free_) );
						if( pool_.size >= max_in_pool )
						{
							release_slice(free_);
						}
						else {
							pool_.store( free_ );
						}

					}
					free_ = s;					
				}
			}
			
		}
		
		size_t carver:: used_slices() const throw()
		{
			return list_.size;
		}
		
		size_t carver:: pool_slices() const throw()
		{
			return pool_.size;
		}
		
	}
}
