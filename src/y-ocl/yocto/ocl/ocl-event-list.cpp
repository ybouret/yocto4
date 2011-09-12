#include "yocto/ocl/event.hpp"
#include "yocto/core/base/locate.h"
#include "yocto/ocl/exception.hpp"

namespace yocto
{
	
	namespace ocl
	{
		
		
		size_t Event::List:: size()     const throw() { return size_; }
		size_t Event::List:: capacity() const throw() { return maxi_; }
		
		const char * Event:: List:: name() const throw() { return "ocl::Event::List"; }
		
		static inline
		cl_event * __event_acquire( size_t &n )
		{
			if( n > 0 )
			{
				static memory::allocator &mgr = Core::BaseAllocator::instance();
				return mgr.acquire_as<cl_event>( n );
			}
			else
				return NULL;
		}
		
		static inline 
		void __event_release( cl_event * &event ) throw()
		{
			if( event )
			{
				static memory::allocator &mgr = * Core::BaseAllocator::location();
				mgr.release_as<cl_event>( event );
			}
			assert( NULL == event );
		}
		
		Event:: List:: List() throw() : size_(0), maxi_(0), event_(NULL)
		{
		}
		
		Event:: List:: List( size_t n ) : size_(0), maxi_(n), event_( __event_acquire( maxi_) )
		{
		}
		
		Event:: List:: List( const List &other ) : size_( 0 ), maxi_( other.size_ ), event_( __event_acquire(maxi_) )
		{
			
			while( size_ < other.size_ )
			{
				assert( event_ );
				assert( size_ < maxi_ );
				(void) clRetainEvent( ( event_[size_] = other.event_[size_] ) );
				++size_;
			}
			
		}
		
		Event:: List:: ~List() throw() { release(); }
		
		void Event::List:: free() throw()
		{
			assert( size_ <= maxi_ );
			while( size_ > 0 )
			{
				assert( event_ );
				(void) clReleaseEvent( event_[ --size_ ] );
			}
		}
		
		void Event:: List:: release() throw()
		{
			if( maxi_ )
			{
				assert( event_ );
				free();
				__event_release(event_);
				maxi_ = 0;
			}
			assert( NULL == event_ );
			assert( 0    == maxi_  );
			assert( 0    == size_  );
		}
		
		void Event:: List:: reserve( size_t n )
		{
			if( n > 0 )
				do_reserve_( n );
		}
		
		void Event:: List:: do_reserve_( size_t n )
		{
			assert( n > 0 );
			size_t    new_maxi  = maxi_ + n;
			cl_event *new_event = __event_acquire( new_maxi );
			memcpy( new_event, event_, size_ * sizeof( cl_event ) );
			__event_release( event_ );
			maxi_  = new_maxi;
			event_ = new_event;
		}
		
		static
		int __compare_events( const void *lhs, const void *rhs ) throw()
		{
			assert( lhs );
			assert( rhs );
			return ( (const char *)lhs ) - ( (const char*)rhs);
		}
		
		bool Event::List:: look_up( const cl_event event, size_t &index ) const throw()
		{
			return YCE_BSearch( &event, event_, size_, sizeof(cl_event), __compare_events, &index) != 0;
		}
		
		
		void Event::List:: insert( const cl_event event )
		{
			size_t item_index = 0;
			if( look_up( event, item_index ) )
				throw exception("%s.insert( multiple event )", name() );
			
			if( size_ > maxi_ ) 
			{
				try 
				{
					do_reserve_(1);
				}
				catch (...) 
				{
					(void) clReleaseEvent(event);
					throw;
				}
				
			}
			
			YCE_BInsert( &event, item_index, event_, &size_, sizeof(cl_event) );
			
		}
		
		
		void Event::List:: insert( const Event &ev )
		{
			const cl_event event      = * ev.hEvent_;
			size_t         item_index = 0;
			if( look_up( event, item_index ) )
				throw exception("%s.insert( multiple event )", name() );
			if( size_ > maxi_ ) do_reserve_( 1 );
			YCE_BInsert( &event, item_index, event_, &size_, sizeof(cl_event) );
			(void) clRetainEvent( event );
		}
		
		void Event:: List:: WaitFor()
		{
			const cl_int err = clWaitForEvents( num_events(), (*this)() );
			if( CL_SUCCESS != err )
				throw Exception( err, "clWaitForEvents" );
		}
		
	}
	
}
