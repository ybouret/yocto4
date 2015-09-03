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
            Y_LOCKED_ENGINE
            (
             std::cerr << "[engine] init..." << std::endl
             );
            
            try
            {
                
                //______________________________________________________________
                //
                // prepare all worker threads
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
                
                
            }
            catch(...)
            {
                quit();
                throw;
            }
        }
        
        
        void engine:: quit() throw()
        {
            Y_LOCKED_ENGINE
            (
             std::cerr << "[engine] quit..." << std::endl;
             dying = true
             );
            
            //__________________________________________________________________
            //
            // and finally finnish all threads
            //__________________________________________________________________
            Y_LOCKED_ENGINE
            (
             std::cerr << "[engine] wait for all threads to return..." << std::endl
             );
            workers.finish();
            
            Y_LOCKED_ENGINE
            (
             std::cerr << "[engine] ...and done!" << std::endl
             );
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
            Y_LOCKED_ENGINE
            (
             std::cerr << "[engine] --> start MASTER" << std::endl;
             );
        }
        
        void engine:: worker_loop() throw()
        {
            Y_LOCKED_ENGINE
            (
             std::cerr << "[engine] --> start worker" << std::endl;
             );
        }
    }
}



