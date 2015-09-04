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
completed(),                      \
access(workers.access),           \
dying(false),                     \
tasks(),                          \
activ(),                          \
alive(activ.size),                \
tpool(),                          \
juuid(1),                         \
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

            {
                YOCTO_LOCK(access);
                std::cerr << "[engine] initialize" << std::endl;
            }

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
                            std::cerr << "[engine] threads are synchronized." << std::endl;

                            //__________________________________________________
                            //
                            // threads placements
                            //__________________________________________________

                            // main thread on root CPU
                            std::cerr << "[engine] assigning main thread:" << std::endl;
                            std::cerr << "[engine] "; assign_current_thread_on( cpu_index_of(0) );

                            // regular dispatch of workers
                            std::cerr << "[engine] assigning workers thread:" << std::endl;
                            size_t iThread = 0;
                            for(thread *thr = workers.head; thr->next; thr=thr->next)
                            {
                                std::cerr << "[engine] "; thr->on_cpu( cpu_index_of(iThread++) );
                            }

                            // last thread is master, on root CPU, doesn't work a lot
                            std::cerr << "[engine] assigning MASTER thread:" << std::endl;
                            std::cerr << "[engine] "; workers.tail->on_cpu( cpu_index_of(0) );

                            std::cerr << "[engine] ready." << std::endl << std::endl;
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
            std::cerr << std::endl;
            std::cerr << "[engine] turning off !" << std::endl;
            //__________________________________________________________________
            //
            // remove pending tasks
            //__________________________________________________________________
            (bool&)dying = true;
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
            access.lock();
            if(activ.size>0)
            {
                // at least one thread is running...
                completed.wait(access);
            }
            access.unlock();

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
            ++(size_t&)ready;
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

            //__________________________________________________________________
            //
            //
            // master thread is waken up !
            //
            //__________________________________________________________________
            if(dying)
            {
                if(alive>0)
                    goto WAIT_FOR_WORK_DONE;
                std::cerr << "[engine] Master is done." << std::endl;
                completed.broadcast();
                access.unlock();
                return;
            }

            std::cerr << "[engine] work done. Remaining #tasks=" << tasks.size << "." << std::endl;
            if(tasks.size)
            {
                more_work.signal();
            }
            else
            {
                if(alive<=0)
                {
                    std::cerr << "[engine] Completed !" << std::endl;
                    completed.broadcast();
                }
            }

            goto WAIT_FOR_WORK_DONE;
        }


        ////////////////////////////////////////////////////////////////////////
        //
        // Worker Loop
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
            ++(size_t&)ready;
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
            std::cerr << "[engine] ===> " << thread_name <<  std::endl;
            if(dying)
            {
                std::cerr << "[engine] stop " << thread_name << std::endl;
                access.unlock();
                return;
            }

            if(tasks.size>0)
            {

                task *todo = tasks.pop_front(); // extract next task
                activ.push_back(todo);          // set it in active state
                access.unlock();                // other threads can run
                try
                {
                    todo->work(access);         // do the job, UNLOCKED...
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

        void  engine:: activate(task *t) throw()
        {
            ++juuid;
            tasks.push_back(t);
            more_work.signal();
        }


        job_id engine:: enqueue(const job &J)
        {
            YOCTO_LOCK(access);
            task *t = query_task();
            try { new(t) task(juuid,J); } catch(...) { tpool.store(t); throw; }
            activate(t);
            return t->uuid;
        }

        //! wait until all jobs are done
        void engine:: flush() throw()
        {
            access.lock();
            if(alive>0)
            {
                completed.wait(access);
            }
            access.unlock();
        }

        bool engine:: is_done(const job_id j) const throw()
        {
            YOCTO_LOCK(access);
            for(const task *t=tasks.head;t;t=t->next)
            {
                if(j==t->uuid)
                    return false;
            }

            for(const task *t=activ.head;t;t=t->next)
            {
                if(j==t->uuid)
                    return false;
            }
            
            return true;
        }

    }
}


