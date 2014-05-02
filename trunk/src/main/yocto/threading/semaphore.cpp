#include "yocto/threading/semaphore.hpp"

namespace yocto {
	
	namespace threading
	{
		semaphore:: semaphore( const uint32_t initialPermits ) throw() :
		count_( initialPermits ),
		waiters_count_( 0 ),
		count_lock_( "sempahore" ),
		count_nonzero_()
		{
		}
		
		
		semaphore:: ~semaphore() throw()
		{
			//--MUST be in a coherent status
		}
		
		
		void semaphore:: wait() throw()
		{
			count_lock_.lock();
			++waiters_count_;
			while( count_ == 0 ) {
				count_nonzero_.wait( count_lock_ );
			}
			--waiters_count_;
			--count_;
			count_lock_.unlock();
		}
		
		void semaphore:: post() throw()
		{
			count_lock_.lock();
			if( waiters_count_ > 0 )
			{
				count_nonzero_.signal();
			}
			++count_;
			count_lock_.unlock();
		}
	}
}
