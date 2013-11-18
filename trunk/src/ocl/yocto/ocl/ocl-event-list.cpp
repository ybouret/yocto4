#include "yocto/ocl/event.hpp"
#include "yocto/ocl/exception.hpp"
#include "yocto/core/locate.hpp"

//#include <iostream>

namespace yocto
{
	
	namespace ocl
	{
		size_t Event::List:: size()     const throw() { return size_; }
		size_t Event::List:: capacity() const throw() { return maxi_; }
		
		const char * Event:: List:: name() const throw() { return "ocl::Event::List"; }
		
		static inline
		cl_event * __event_acquire( size_t &num_events )
		{
			//std::cerr << "event.acquire(" << num_events << ")" << std::endl;
			return memory::kind<MemoryType>::acquire_as<cl_event>(num_events);
		}
		
		static inline 
		void __event_release( cl_event * &events, size_t &num_events ) throw()
		{
			//std::cerr << "event.release(" << num_events << ")" << std::endl;
			memory::kind<MemoryType>::release_as<cl_event>(events,num_events);
			assert( NULL == events     );
			assert( 0    == num_events );
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
			free();
			__event_release(event_, maxi_);
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
			__event_release( event_, maxi_ );
			maxi_  = new_maxi;
			event_ = new_event;
		}
		
		static inline
		int __compare_events( const void *lhs, const void *rhs ) throw()
		{
			assert( lhs );
			assert( rhs );
			return ( (const char *)lhs ) - ( (const char*)rhs);
		}
		
		bool Event::List:: look_up( const cl_event event, size_t &index ) const throw()
		{
			return core::locate( &event, event_, size_, index, __compare_events);
		}
		
		
		void Event::List:: insert( const cl_event event )
		{
			size_t item_index = 0;
			if( look_up( event, item_index ) )
				throw exception("%s.insert( multiple event )", name() );
			
			if( size_ >= maxi_ ) 
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
			
			core::insert<cl_event>( &event, event_, size_, item_index );
			++size_;
		}
		
		
		void Event::List:: insert( const Event &ev )
		{
			const cl_event event      = *ev;
			size_t         item_index = 0;
			if( look_up( event, item_index ) )
				throw exception("%s.insert( multiple event )", name() );
			if( size_ > maxi_ ) do_reserve_( 1 );
			core::insert<cl_event>( &event, event_, size_, item_index );
			++size_;
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
