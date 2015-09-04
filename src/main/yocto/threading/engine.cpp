#include "yocto/threading/engine.hpp"
#include <iostream>

namespace yocto
{
    namespace threading
    {

        engine:: task:: ~task() throw() {}
        engine:: task:: task( const job_id I, const job &J ) :
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
                    workers.launch(worker_call,this);
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
                        if(ready<=size)
                        {
                            access.unlock();
                        }
                        else
                        {
                            assert(1+size==ready);
                            std::cerr << "[engine] threads are synchronized" << std::endl;

                            //__________________________________________________
                            //
                            // threads placements
                            //__________________________________________________
                            std::cerr << "[engine] assigning main thread" << std::endl;
                            assign_current_thread_on( cpu_index_of(0) );

                            std::cerr << "[engine] assigning workers thread" << std::endl;
                            size_t iThread = 0;
                            for(thread *thr = workers.head; thr->next; thr=thr->next)
                            {
                                thr->on_cpu( cpu_index_of(iThread++) );
                            }
                            std::cerr << "[engine] assigning MASTER thread" << std::endl;
                            workers.tail->on_cpu( cpu_index_of(0) );

                            std::cerr << "[engine] all threads are ready" << std::endl << std::endl;
                            ready = size;
                            access.unlock();
                            break;
                        }
                    }
                }
            }
            catch(...)
            {
                quit();
                throw;
            }
        }


        void engine:: quit() throw()
        {

            access.lock();
            std::cerr << "[engine] quit..." << std::endl;
            //__________________________________________________________________
            //
            // remove pending tasks
            //__________________________________________________________________
            dying = true;
            std::cerr << "[engine] kill #pending_tasks=" << tasks.size << std::endl;
            while(tasks.size>0)
            {
                task *t = tasks.pop_back();
                t->~task();
                object::release1<task>(t);
            }
            access.unlock();

            //__________________________________________________________________
            //
            // waiting for active tasks to complete...
            // TODO: use flush mechanism...
            //__________________________________________________________________
            while(true)
            {
                more_work.broadcast();
                YOCTO_LOCK(access);
                if(activ.size<=0)
                    break;
            }

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

            while(tpool.size>0) object::release1<task>(tpool.query());

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

#include "yocto/code/bin2name.hpp"

namespace yocto
{
    namespace threading
    {


        ////////////////////////////////////////////////////////////////////////
        //
        // Master Loop
        //
        ////////////////////////////////////////////////////////////////////////
        void engine:: master_loop() throw()
        {
            //__________________________________________________________________
            //
            //
            // Waiting for everyone to be ready
            //
            //__________________________________________________________________
            access.lock();
            ++ready;
            const thread::handle_t         thread_handle = thread::get_current_handle();
            const bin2id<thread::handle_t> thread_bin_id = &thread_handle;
            const char                    *thread_name   = thread_bin_id.value;
            std::cerr << "[engine] Master name is " << thread_name << std::endl;

        WAIT_FOR_WORK_DONE:
            //__________________________________________________________________
            //
            //
            // wait on the LOCKED access that some work was done
            //
            //__________________________________________________________________
            work_done.wait(access);
            std::cerr << "[engine] work done. Remaining #tasks=" << tasks.size << std::endl;

            //__________________________________________________________________
            //
            //
            // master thread is waken up !
            //
            //__________________________________________________________________
            if(dying)
            {
                std::cerr << "[engine] Master is done" << std::endl;
                access.unlock();
                return;
            }

            if(tasks.size)
            {
                more_work.signal();
            }


            goto WAIT_FOR_WORK_DONE;
        }


        ////////////////////////////////////////////////////////////////////////
        //
        // WOrker Loop
        //
        ////////////////////////////////////////////////////////////////////////
        void engine:: worker_loop() throw()
        {
            //__________________________________________________________________
            //
            //
            // Waiting for all threads to be ready
            //
            //__________________________________________________________________
            access.lock();
            ++ready;
            const thread::handle_t         thread_handle = thread::get_current_handle();
            const bin2id<thread::handle_t> thread_bin_id = &thread_handle;
            const char                    *thread_name   = thread_bin_id.value;
            std::cerr << "[engine] Worker name is " << thread_name << std::endl;

        WAIT_FOR_MORE_WORK:
            //__________________________________________________________________
            //
            //
            // Waiting for being ready on the LOCKED access
            //
            //__________________________________________________________________
            more_work.wait(access);

            //__________________________________________________________________
            //
            //
            // worker is woken up, access is LOCKED
            //
            //__________________________________________________________________
            std::cerr << "[engine] ==> " << thread_name <<  std::endl;
            assert(ready>0);
            --ready;
            if(dying)
            {
                std::cerr << "[engine] stop worker..." << std::endl;
                access.unlock();
                return;
            }

            std::cerr << "[engine] do something" << std::endl;
            if(tasks.size>0)
            {

                task *todo = tasks.pop_front(); // extract next task
                activ.push_back(todo);          // set it in active state
                access.unlock();                // other threads can run
                try
                {
                    todo->work(access);             // do the job, UNLOCKED...
                }
                catch(...)
                {
                }
                access.lock();                  // lock access for ops
                activ.unlink(todo)->~task();    // remove task
                tpool.store(todo);              // keep memory
            }
            //__________________________________________________________________
            //
            //
            // loop is done, access is LOCKED
            //
            //__________________________________________________________________
            ++ready;
            work_done.signal();
            goto WAIT_FOR_MORE_WORK;
        }
    }
}

namespace yocto
{
    namespace threading
    {

        engine::task * engine::query_task()
        {
            if(tpool.size>0)
            {
                return tpool.query();
            }
            else
            {
                return object::acquire1<task>();
            }
        }
        
        job_id engine:: enqueue(const job &J)
        {
            YOCTO_LOCK(access);
            task *t = query_task();
            try { new(t) task(juuid,J); } catch(...) { tpool.store(t); throw; }
            ++juuid; tasks.push_back(t);
            more_work.signal();
            return t->uuid;
        }
    }
}


