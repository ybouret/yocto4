#include "yocto/threading/engine.hpp"
#include <iostream>

namespace yocto
{
    namespace threading
    {
#define Y_LOCKED_ENGINE(CODE) do { \
YOCTO_LOCK(access);                \
CODE;                              \
} while(false)
        
#define Y_THREADING_ENGINE_CTOR() \
workers("engine"),                \
more_work(),                      \
work_done(),                      \
access(workers.access),           \
dying(false),                     \
ready(0)
        
        engine:: engine() : layout(),
        Y_THREADING_ENGINE_CTOR()
        {
            init();
        }
        
        engine:: engine(const size_t num_threads, const size_t threads_offset) :
        layout(num_threads,threads_offset),
        Y_THREADING_ENGINE_CTOR()
        {
            init();
        }
        
        
        engine:: ~engine() throw()
        {
            quit();
        }
        
        
        void engine:: init()
        {
            Y_LOCKED_ENGINE(std::cerr << "[engine] init..." << std::endl);
            
            try
            {
                
                //______________________________________________________________
                //
                // prepare all worker threads:0..size-1
                //______________________________________________________________
                for(size_t i=0;i<size;++i)
                {
                    workers.launch(worker_call, this);
                }
                
                //______________________________________________________________
                //
                // prepare master thread
                //______________________________________________________________
                workers.launch(master_call,this);
                
                //______________________________________________________________
                //
                // wait for first synchronization
                //______________________________________________________________
                while(true)
                {
                    if(access.try_lock())
                    {
                        if(ready>size)
                        {
                            assert(1+size==ready);
                            access.unlock();
                            break;
                        }
                        else
                        {
                            access.unlock();
                        }
                    }
                }
                Y_LOCKED_ENGINE(std::cerr << "[engine] threads are ready" << std::endl);
                
                
            }
            catch(...)
            {
                quit();
                throw;
            }
        }
        
        
        void engine:: quit() throw()
        {
            Y_LOCKED_ENGINE(std::cerr << "[engine] quit..." << std::endl);
            Y_LOCKED_ENGINE(dying=true);
            
            //__________________________________________________________________
            //
            // ok, all jobs are done...
            //__________________________________________________________________
            
            more_work.broadcast();
            work_done.broadcast();
            //__________________________________________________________________
            //
            // and finally finnish all threads
            //__________________________________________________________________
            Y_LOCKED_ENGINE(std::cerr << "[engine] wait for all threads to return..." << std::endl);
            workers.finish();
            
            Y_LOCKED_ENGINE(std::cerr << "[engine] ...and done!" << std::endl);
        }
        
        
        
        
        void engine:: worker_call(void *args) throw()
        {
            assert(args);
            static_cast<engine *>(args)->worker_loop();
        }
        
        void engine:: master_call(void *args) throw()
        {
            assert(args);
            static_cast<engine *>(args)->master_loop();
        }
    }
    
}


namespace yocto
{
    namespace threading
    {
        
        void engine:: master_loop() throw()
        {
            Y_LOCKED_ENGINE(std::cerr << "[engine] --> start MASTER" << std::endl);
            
            //__________________________________________________________________
            //
            //
            // Waiting for everyone to be ready
            //
            //__________________________________________________________________
            access.lock();
            ++ready;
            
            //__________________________________________________________________
            //
            //
            // wait on the LOCKED access that some work was done
            //
            //__________________________________________________________________
            work_done.wait(access);

            //__________________________________________________________________
            //
            //
            // master thread is waken up !
            //
            //__________________________________________________________________
            if(dying)
            {
                std::cerr << "[engine] MASTER is done" << std::endl;
                access.unlock();
                return;
            }
            
            access.unlock();
        }
        
        void engine:: worker_loop() throw()
        {
            Y_LOCKED_ENGINE(std::cerr << "[engine] --> start worker" << std::endl);
            
            //__________________________________________________________________
            //
            //
            // Waiting for all threads to be ready
            //
            //__________________________________________________________________
            access.lock();
            ++ready;

            //__________________________________________________________________
            //
            //
            // Waiting for all threads to be ready on the LOCKED access
            //
            //__________________________________________________________________
            more_work.wait(access);
            
            
            
            access.unlock();
            
        }
    }
}



