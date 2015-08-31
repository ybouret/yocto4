#ifndef YOCTO_THREADING_SERVER_INCLUDED
#define YOCTO_THREADING_SERVER_INCLUDED 1

#include "yocto/threading/threads.hpp"
#include "yocto/threading/condition.hpp"
#include "yocto/threading/layout.hpp"
#include "yocto/functor.hpp"


namespace yocto
{
    namespace threading
    {

        class server : public layout
        {
        public:
            typedef uint64_t task_id;
            explicit server();
            explicit server(size_t num_threads, size_t thread_offset = 0);
            virtual ~server() throw();
            typedef functor<void,TL1(lockable&)> job;

            //! enqueue a new job
            /**
             without flush, there is no guarantee that the job is done...
             will throw on memory failure...
             */
            task_id enqueue(const job &J);

            //! wait until all jobs are done
            void    flush() throw();

            //! look up in current or pending tasks
            bool    is_done(const task_id I) const throw();

        private:
            threads   workers; //!< internal threads, set to layout.size
            condition process; //!< used to wait for job to do


        public:
            mutex    &access; //!< workers.access

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(server);
            class task
            {
            public:
                task         *next;
                task         *prev;
                const task_id uuid;
                job           work;

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(task);
                task(const task_id I, const job &J);

                template <typename OBJECT_POINTER,typename METHOD_POINTER> inline
                task(const task_id I, OBJECT_POINTER host, METHOD_POINTER method) :
                next(0),prev(0),uuid(I),work(host,method) {}


                ~task() throw();
                friend class server;
            };

            core::list_of<task> tasks;  //!< tasks to process
            core::list_of<task> activ;  //!< running tasks
            core::pool_of<task> tpool;  //!< pool of dangling tasks
            task_id             tuuid;  //!< for tasks labelling...
            size_t              ready;  //!< for first synchro
            bool                dying;  //!< when ending...

            void    initialize();
            void    terminate() throw();
            void    run() throw();
            static  void thread_run(void*) throw();
            task   *query_task();

        public:
            template <typename OBJECT_POINTER,typename METHOD_POINTER> inline
            task_id enqueue(OBJECT_POINTER host, METHOD_POINTER method)
            {
                YOCTO_LOCK(access);
                // get a new task
                task *t = query_task();
                try
                {
                    // try tro create it
                    ++tuuid;
                    new (t) task(tuuid,host,method);
                    tasks.push_front(t);
                }
                catch(...)
                {
                    // cleanup
                    tpool.store(t);
                    throw;
                }
                // wake up some thread if possible
                process.signal();
                return t->uuid;
            }
        };
        
        
    }
}

#endif

