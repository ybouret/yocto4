#include "yocto/threading/server.hpp"
#include <iostream>

namespace yocto
{

    namespace threading
    {

#define Y_THREADING_SERVER(CODE) do { \
YOCTO_LOCK(access);\
CODE;\
} while(false)
        ////////////////////////////////////////////////////////////////////////
        //
        // construction
        //
        ////////////////////////////////////////////////////////////////////////

#define Y_THREADING_SERVER_CTOR() \
workers("server"),\
access(workers.access)

        server:: server() :
        layout(),
        Y_THREADING_SERVER_CTOR()
        {

        }

        server:: server(const size_t num_threads, const size_t threads_offset) :
        layout(num_threads,threads_offset),
        Y_THREADING_SERVER_CTOR()
        {

        }


        server:: ~server() throw()
        {
            terminate();
        }

        ////////////////////////////////////////////////////////////////////////
        //
        // termination
        //
        ////////////////////////////////////////////////////////////////////////
        void server:: terminate() throw()
        {
            Y_THREADING_SERVER(std::cerr<<"[server] terminate"<<std::endl);

        }


        ////////////////////////////////////////////////////////////////////////
        //
        // initialization
        //
        ////////////////////////////////////////////////////////////////////////
        void server:: initialize()
        {
            Y_THREADING_SERVER(std::cerr<<"[server] initialize"<<std::endl);

            try
            {
                //______________________________________________________________
                //
                // prepare all threads
                //______________________________________________________________
                for(size_t i=0;i<size;++i)
                {
                    workers.launch(thread_entry, this);
                }
            }
            catch(...)
            {
                terminate();
                throw;
            }
        }

    }
}



#if 0
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
        process(),
        synchro(),
        access( workers.access  ),
        tasks(),
        activ(),
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
                //______________________________________________________________
                //
                // prepare all threads
                //______________________________________________________________
                for(size_t i=0;i<size;++i)
                {
                    workers.launch(thread_entry, this);
                }

                //______________________________________________________________
                //
                // wait for first syncro
                //______________________________________________________________
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
                            //__________________________________________________
                            //
                            // Placement
                            //__________________________________________________
                            std::cerr << "[server] control" << std::endl;
                            assign_current_thread_on( cpu_index_of(0));

                            std::cerr << "[server] workers" << std::endl;
                            size_t iThread = 0;
                            for(thread *thr = workers.head; thr; thr=thr->next)
                            {
                                thr->on_cpu( cpu_index_of(iThread++) );
                            }


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
            // dying time has come: kill extraneous tasks
            //------------------------------------------------------------------
            {
                YOCTO_LOCK(access);
                dying = true;
                while( tasks.size )
                {
                    task *t = tasks.pop_back();
                    t->~task();
                    object::release1<task>(t);
                }
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
            assert(activ.size<=0);

            while(tpool.size) object::release1<task>(tpool.query());


        }

        void server:: flush() throw()
        {
            access.lock();
            std::cerr << "[server] flushing" << std::endl;
            synchro.wait(access);
            //std::cerr << "[server] remaining tasks=" << tasks.size << std::endl;
            access.unlock();
        }

        void server:: enqueue( const job &J )
        {
            YOCTO_LOCK(access);
            //std::cerr << "[server] new task (in cache: " << tasks.size << ")" << std::endl;
            task *t = tpool.size ? tpool.query() : object::acquire1<task>();
            try
            {
                //______________________________________________________________
                //
                // create the new task
                //______________________________________________________________
                new (t) task(J);
                tasks.push_front(t);

            }
            catch(...)
            {
                tpool.store(t);
                throw;
            }

            //__________________________________________________________________
            //
            // wake up some thread if needed
            //__________________________________________________________________
            process.signal();
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
                std::cerr << "[server] launching thread!" << std::endl;
            }

            access.lock();
            ++ready;
        PROCESS_TASKS:
            //------------------------------------------------------------------
            // Wait on the LOCKED access
            //------------------------------------------------------------------
            process.wait(access); // and return an unlocked mutex

        CHECK_BEHAVIOR:
            //------------------------------------------------------------------
            // LOCKED return
            //------------------------------------------------------------------
            if(dying)
            {
                std::cerr << "[server] quit thread!" << std::endl;
                access.unlock();
                return;
            }

            //------------------------------------------------------------------
            // is there a task to run
            //------------------------------------------------------------------
            if(tasks.size)
            {
                {
                    //YOCTO_LOCK(access);
                    //std::cerr << "[server] running task out of " << tasks.size << std::endl;
                }
                task *todo = tasks.pop_back();
                activ.push_back(todo);
                access.unlock();
                todo->work();
                access.lock();
                
                activ.unlink(todo);
                todo->~task();
                tpool.store(todo);
                
                goto CHECK_BEHAVIOR;
            }
            
            
            synchro.signal();
            goto PROCESS_TASKS;
        }
        
    }
    
}
#endif
