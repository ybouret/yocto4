//! \file

#ifndef YOCTO_RUNNABLE_INCLUDED
#define YOCTO_RUNNABLE_INCLUDED 1

#include "yocto/threading/thread.hpp"



namespace yocto {
	
	
	namespace threading
	{
		//! base class for run() in a thread
		class runnable
        {
        public:
            virtual ~runnable() throw();  //!< join() MUST be called if start() was called
            void     start();             //!< call the run() command in one new thread
            void     finish() throw();    //!< wait for thread to finish
            
        protected:
            explicit runnable(mutex &shared) throw();
            
        private:
            virtual void run() throw() = 0;
            
            thread     *thr;
            static void execute( void * ) throw();
            void        cleanup() throw();
        };
        
        
	}
	
}

#endif
