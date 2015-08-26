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
             */
            task_id enqueue(const job &J);

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
                ~task() throw();
                friend class server;
            };

            core::list_of<task> tasks;  //!< tasks to process
            core::list_of<task> activ;  //!< running tasks
            core::pool_of<task> tpool;  //!< pool of empty tasks
            task_id             tuuid;  //!< for tasks labelling...
            size_t              ready;  //!< for first synchro
            bool                dying;  //!< when ending...
            
            void    initialize();
            void    terminate() throw();
            void    run() throw();
            static  void thread_run(void*) throw();
        };


#if 0
        //! a task server
        class server : public layout
        {
        public:
            explicit server();
            virtual ~server() throw();
            
            //! a job a a full fledge functor
            typedef functor<void,null_type> job;
        
            //! dynamic task, containing the job to do
            class task
            {
            public:
                task   *next;
                task   *prev;
                job     work;
                
            private:
                task( const job & );
                ~task() throw();
                YOCTO_DISABLE_COPY_AND_ASSIGN(task);
                friend class threading::server;
            };
            
            void enqueue( const job &J );
            void flush() throw();
            
        private:
            threads   workers;
            condition process;
            condition synchro;
            
        public:
            mutex  &access;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(server);
            core::list_of<task> tasks;  //!< tasks to process
            core::list_of<task> activ;  //!< tasks being processed
            core::pool_of<task> tpool;  //!< memory
            size_t              ready;  //!< for first sync
            bool                dying;  //!< terminating
            
            //! called with 'this', call the run() function
            static void thread_entry(void *) throw();
            
            void run() throw();
            
            void initialize();
            void terminate() throw();
            
        };
#endif
        
    }
}

#endif

