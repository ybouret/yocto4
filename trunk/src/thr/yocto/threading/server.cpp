#include "yocto/threading/server.hpp"
#include <iostream>

namespace yocto
{
    namespace threading
    {
        
        server::task:: ~task() throw() {}
        
        server::task:: task( const server::job &J ) :
        next(0),
        prev(0),
        work(J)
        {
            
        }
        
        
        server:: ~server() throw()
        {
            terminate();
        }
        
        
        server:: server() :
        layout(),
        workers( "server", size ),
        access( workers.access ),
        tasks(),
        tpool(),
        ready(0),
        dying(false)
        {
            initialize();
        }
        
        void server:: initialize()
        {
            try
            {
                //-- prepare all threads
                for(size_t i=0;i<size;++i)
                {
                    workers.launch(thread_entry, this);
                }
                
                //-- wait for first syncro
                for(;;)
                {
                    if( access.try_lock() )
                    {
                        if(ready<size)
                        {
                            access.unlock();
                            continue;
                        }
                        else
                        {
                            std::cerr << "[server] threads are ready" << std::endl;
                            access.unlock();
                            break;
                        }
                    }
                }
            }
            catch(...)
            {
                terminate();
                throw;
            }
        }
        
        void server:: terminate() throw()
        {
            //------------------------------------------------------------------
            // dying time has come
            //------------------------------------------------------------------
            {
                YOCTO_LOCK(access);
                dying = true;
            }
            
            //------------------------------------------------------------------
            // broadcast until everybody is done
            //------------------------------------------------------------------
            for(;;)
            {
                process.broadcast();
                YOCTO_LOCK(access);
                if(activ.size<=0)
                    break;
            }
            
            //------------------------------------------------------------------
            // cleanup
            //------------------------------------------------------------------
            workers.finish();
            
        }
        
        void server:: enqueue( const job &J )
        {
            YOCTO_LOCK(access);
            task *t = tpool.size ? tpool.query() : object::acquire1<task>();
            try
            {
                //______________________________________________________________
                //
                // create the new task
                //______________________________________________________________
                new (t) task(J);
                tasks.push_front(t);
                
                //______________________________________________________________
                //
                // try to start it
                //______________________________________________________________
                if(activ.size<size)
                {
                    // there should be a waiting thread
                }
                
            }
            catch(...)
            {
                tpool.store(t);
                throw;
            }
            
        }
        
        
        void server:: thread_entry(void *args) throw()
        {
            assert(args);
            static_cast<server *>(args)->run();
        }
        
        
        void server:: run() throw()
        {
            {
                YOCTO_LOCK(access);
                std::cerr << "Launching Thread!" << std::endl;
            }
            
            access.lock();
            ++ready;
        PROCESS_TASK:;
            //------------------------------------------------------------------
            // Wait on the LOCKED access
            //------------------------------------------------------------------
            process.wait(access); // and return an unlocked mutex
            
            //------------------------------------------------------------------
            // Locked Return
            //------------------------------------------------------------------
            if(dying)
            {
                std::cerr << "[server] quit thread!" << std::endl;
                access.unlock();
                return;
            }
        }
        
    }
    
}
