#include "yocto/threading/runnable.hpp"
#include <new>
#include <cstring>

namespace yocto {
	
	namespace threading
	{
        
		void runnable:: cleanup() throw()
		{
		}
		
		runnable:: runnable( mutex &access ) throw() :
		thr(NULL)
		{
			YOCTO_LOCK(access);
            thr = thread::create_with(access);
		}
		
		runnable:: ~runnable() throw()
		{
            assert(thr);
            YOCTO_LOCK(thr->access);
            thread::destruct(thr);
		}
		
		void runnable:: start()
		{
            assert(thr);
            YOCTO_LOCK(thr->access);
            runnable *self = this;
            thr->launch(execute,self);
        }
		
        void runnable:: finish() throw()
        {
            assert(thr);
            YOCTO_LOCK(thr->access);
            
        }
        
		void runnable::execute( void *args ) throw()
		{
			assert( args );
			runnable *host = static_cast<runnable *>( args );
			host->run();
		}
		
        
		
	}
}
