#include "yocto/threading/server.hpp"
#include <iostream>


namespace yocto
{
    namespace threading
    {

        server:: task:: ~task() throw() {}
        server:: task:: task( const task_id I, const job &J ) :
        next(0),
        prev(0),
        uuid(I),
        work(J)
        {

        }

    }

}



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
workers("server"),                \
process(),                        \
access(workers.access),           \
tasks(),                          \
activ(),                          \
tpool(),                          \
tuuid(0),                         \
ready(0),                         \
dying(false)


        server:: server() :
        layout(),
        Y_THREADING_SERVER_CTOR()
        {
            initialize();
        }

        server:: server(const size_t num_threads, const size_t threads_offset) :
        layout(num_threads,threads_offset),
        Y_THREADING_SERVER_CTOR()
        {
            initialize();
        }


        server:: ~server() throw()
        {
            terminate();
        }

        bool server::is_done(const task_id I) const throw()
        {
            YOCTO_LOCK(access);
            for(const task *t=tasks.head;t;t=t->next)
            {
                if(I==t->uuid) return false;
            }

            for(const task *t=activ.head;t;t=t->next)
            {
                if(I==t->uuid) return false;
            }

            return true;
        }


        ////////////////////////////////////////////////////////////////////////
        //
        // termination
        //
        ////////////////////////////////////////////////////////////////////////
        void server:: terminate() throw()
        {
            Y_THREADING_SERVER(std::cerr<<std::endl<<"[server] will terminate"<<std::endl);

            //__________________________________________________________________
            //
            // kill pending tasks
            //__________________________________________________________________
            {
                YOCTO_LOCK(access);
                std::cerr << "[server] kill #pending tasks=" << tasks.size << std::endl;
                dying = true;
                while(tasks.size)
                {
                    task *t = tasks.pop_back();
                    t->~task();
                    object::release1<task>(t);
                }
            }

            //__________________________________________________________________
            //
            // broadcast until everybody left is done
            //__________________________________________________________________
            Y_THREADING_SERVER(std::cerr<<"[server] finishing #active="<<activ.size<<std::endl);
            while(true)
            {
                process.broadcast();
                YOCTO_LOCK(access);
                if(activ.size<=0)
                {
                    break;
                }
            }

            //__________________________________________________________________
            //
            // wait for threads to come back
            //__________________________________________________________________
            workers.finish();

            //__________________________________________________________________
            //
            // kill pooled task
            //__________________________________________________________________
            while(tpool.size)
            {
                object::release1<task>(tpool.query());
            }

        }

        ////////////////////////////////////////////////////////////////////////
        //
        // flushing
        //
        ////////////////////////////////////////////////////////////////////////
        void server:: flush() throw()
        {
            //Y_THREADING_SERVER(std::cerr<<"[server] flushing #all="<<activ.size+tasks.size<<std::endl);
            while(true)
            {
                process.broadcast();
                YOCTO_LOCK(access);
                if(activ.size<=0&&tasks.size<=0)
                {
                    break;
                }
            }
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
                    workers.launch(thread_run, this);
                }

                //______________________________________________________________
                //
                // wait for first synchronization
                //______________________________________________________________
                while(true)
                {
                    if( access.try_lock() )
                    {
                        if(ready<size)
                        {
                            access.unlock();
                        }
                        else
                        {
                            std::cerr << "[server] all threads are synchronized" << std::endl;
                            //__________________________________________________
                            //
                            // thread placement
                            //__________________________________________________
                            std::cerr << "[server] assigning control thread" << std::endl;
                            assign_current_thread_on( cpu_index_of(0));

                            std::cerr << "[server] assigning workers thread" << std::endl;
                            size_t iThread = 0;
                            for(thread *thr = workers.head; thr; thr=thr->next)
                            {
                                thr->on_cpu( cpu_index_of(iThread++) );
                            }
                            std::cerr << "[server] all threads are ready" << std::endl << std::endl;

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


        ////////////////////////////////////////////////////////////////////////
        //
        // running
        //
        ////////////////////////////////////////////////////////////////////////
        void server::thread_run(void *args) throw()
        {
            assert(args);
            static_cast<server *>(args)->run();
        }

        void server:: run() throw()
        {
            Y_THREADING_SERVER(std::cerr << "[server] starting thread" << std::endl);

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
            // Waiting for a task to do or to quit
            // wait on a LOCKED access, unlocked it for other thread
            //
            //__________________________________________________________________
        WAIT_FOR_PROCESS:
            process.wait(access);

            //__________________________________________________________________
            //
            //
            // we come back with a LOCKED access
            //
            //__________________________________________________________________
        CHECK_BEHAVIOR:
            if(dying)
            {
                std::cerr << "[server] leaving thread" << std::endl;
                access.unlock();
                return;
            }
			else 
			{
				std::cerr << "[server] #task=" << tasks.size << ", with thread $" << uint64_t(thread::get_current_handle()) << std::endl;
			}

            if(tasks.size>0)
            {
                //______________________________________________________________
                //
                // something to do...
                //______________________________________________________________
                task *curr = tasks.pop_front();
                activ.push_back(curr);

                //______________________________________________________________
                //
                // unlock access for other thread
                //______________________________________________________________
                access.unlock();

                //______________________________________________________________
                //
                // nobody else can touch current task
                // (but next/prev, doesn't matter yet)
                // do the work...
                //______________________________________________________________
                try
                {
                    curr->work(access);
                }
                catch(...)
                {
                    // TODO: a message ?
                }

                //______________________________________________________________
                //
                // LOCK access to update activ status
                //______________________________________________________________
                access.lock();


                (void)activ.unlink(curr);
                curr->~task();
                tpool.store(curr);


                goto CHECK_BEHAVIOR; // we are LOCKED here
            }


            // access must be LOCKED at this point
            goto WAIT_FOR_PROCESS;

        }

        server::task   *server::query_task()
        {
            return tpool.size ? tpool.query() : object::acquire1<task>();
        }


        server::task_id server::enqueue(const job &J)
        {
            //______________________________________________________________
            //
            // LOCK and get acquire task memory
            //______________________________________________________________
            YOCTO_LOCK(access);
            task *t = query_task();
            try
            {
                //______________________________________________________________
                //
                // create the new task
                //______________________________________________________________
                ++tuuid;
                new (t) task(tuuid,J);
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

            return t->uuid;
        }


    }
}

