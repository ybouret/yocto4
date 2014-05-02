//! \file

#ifndef YOCTO_THREADING_SEMAPHORE_INCLUDED
#define YOCTO_THREADING_SEMAPHORE_INCLUDED 1

#include "yocto/threading/condition.hpp"


namespace yocto {
	
	namespace threading
	{
        //! portable semaphore class
		class semaphore
        {
		public:
			explicit semaphore( const uint32_t initialPermits ) throw();
			virtual ~semaphore() throw();
			
			void wait() throw();
			void post() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(semaphore);
			uint32_t  count_;
			uint32_t  waiters_count_;
			mutex     count_lock_;
			condition count_nonzero_;
		};
	}
}

#endif
