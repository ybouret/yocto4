//! \file

#ifndef YOCTO_THREADING_BARRIER_INCLUDED
#define YOCTO_THREDAING_BARRIER_INCLUDED 1

#include "yocto/threading/condition.hpp"

namespace yocto
{

	namespace threading
	{
		
		//! synchronization by barrier
		class barrier 
		{
		public:
			
			//! default constructor
			/**
				\param threshold_value release the barrier when that many threads are waiting
				\param id identifier for the internal mutex
			 */
			explicit barrier( size_t threshold_value, const char *id ) throw();
			virtual ~barrier() throw();
			
			//! return the mutex name
			const char *name() const throw();
			
			//! true if the last one to wait => everybody runs!
			bool wait() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(barrier);
			const size_t  threshold; /*!< count of threads to wait for         */
			size_t        counter;   /*!< how many threads are waiting so far  */
			size_t        cycle;     /*!< updated after each reached threshold */
			mutex         guard;     /*!< to protect access                    */
			condition     cv;        /*!< condition variable                   */
		};
		
	}
	
}

#endif
