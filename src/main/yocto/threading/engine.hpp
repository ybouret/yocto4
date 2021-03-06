#ifndef YOCTO_THREADING_ENGINE_INCLUDED
#define YOCTO_THREADING_ENGINE_INCLUDED 1

#include "yocto/threading/threads.hpp"
#include "yocto/threading/condition.hpp"
#include "yocto/threading/layout.hpp"
#include "yocto/counted-object.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
    namespace threading
    {

        typedef uint32_t                     job_id; //!< a unique id
        typedef functor<void,TL1(lockable&)> job;    //!< a job to execute

        //! interface class: job dispatcher
        class dispatcher : public counted_object
        {
        public:
            virtual ~dispatcher() throw();

            virtual job_id enqueue(const job &J ) = 0;
            virtual void   flush()                = 0;
            virtual size_t levels() const throw() = 0;


            //! wrapper for single instruction, multiple data
            /**
             METHOD_POINTER(T &,lockable &)
             */
            template <
            typename OBJECT_POINTER,
            typename METHOD_POINTER,
            typename T
            >
            class jwrapper
            {
            public:

                inline jwrapper(OBJECT_POINTER h, METHOD_POINTER c, T &d ) throw() :
                host(h), call(c), data(d) {}

                inline jwrapper(const jwrapper &other) throw() :
                host(other.host), call(other.call), data(other.data) {}

                inline ~jwrapper() throw() {}

                inline void operator()( lockable &shared_lock )
                {
                    ((*host).*call)(data,shared_lock);
                }

            private:
                OBJECT_POINTER host;
                METHOD_POINTER call;
                T             &data;
                YOCTO_DISABLE_ASSIGN(jwrapper);
            };

            template <typename OBJECT_POINTER,typename METHOD_POINTER,typename T> inline
            job_id enqueue2(OBJECT_POINTER host, METHOD_POINTER method, T &data)
            {
                const jwrapper<OBJECT_POINTER,METHOD_POINTER,T> jw(host,method,data);
                const job   J(jw);
                return enqueue(J);
            }


        protected:
            explicit dispatcher() throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(dispatcher);
        };

        
        //! failsafe sequential dispatcher
        class sequential_dispatcher : public dispatcher
        {
        public:
            virtual ~sequential_dispatcher() throw();
            explicit sequential_dispatcher() throw();

            virtual job_id enqueue(const job &J);
            virtual void   flush();
            virtual size_t levels() const throw();

        private:
            faked_lock access;
            job_id     juuid;
            YOCTO_DISABLE_COPY_AND_ASSIGN(sequential_dispatcher);
        };


        //! manage a given number of threads
        class engine : public layout, public dispatcher
        {
        public:
            //! using YOCTO_THREADING or default settings
            explicit engine(bool setVerbose);

            //! user tailored settings
            explicit engine(const size_t num_threads, const size_t threads_offset,bool setVerbose);
            virtual ~engine() throw();

            //! execute a job
            virtual job_id enqueue( const job &J );

            //! wait until all jobs are done
            virtual void    flush() throw();

            //! this->size
            virtual size_t levels() const throw();

        private:
            threads   workers;    //!< list of threads, layout.size+1
            condition more_work;  //!< workers waiting on it
            condition work_done;  //!< master  waiting on it
            condition completed;  //!< flushing mechanism
            
        public:
            mutex &access;        //!< workers.access
            
        private:
            const bool dying; //!< internal flag to quit properly

            //! internal linked list of tasks
            class task
            {
            public:
                task         *next;
                task         *prev;
                const job_id  uuid;
                job           work;

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(task);

                task(const job_id I, const job &J);

                template <typename OBJECT_POINTER,typename METHOD_POINTER> inline
                task(const job_id I, OBJECT_POINTER host, METHOD_POINTER method) :
                next(0),prev(0),uuid(I),work(host,method) {}

                ~task() throw();
                friend class engine;
            };

            core::list_of<task> tasks;   //!< tasks to process
            const size_t       &pending; //!< task.size
            core::list_of<task> activ;   //!< running tasks
            const size_t       &running; //!< activ.size
            core::pool_of<task> tpool;   //!< pool of dangling tasks
            job_id              juuid;   //!< for tasks labelling...
            const size_t        ready;   //!< internal set-up flag

            YOCTO_DISABLE_COPY_AND_ASSIGN(engine);

            void init();
            void quit() throw();
            
            static void worker_call(void *args) throw();
            void        worker_loop() throw();
            
            static void master_call(void *args) throw();
            void        master_loop() throw();

            task *query_task();
            void  activate(task *t) throw();

        public:

#define YOCTO_THREAD_ENGINE_ENQUEUE_PROLOG() YOCTO_LOCK(access); task *t = query_task()
#define YOCTO_THREAD_ENGINE_ENQUEUE_EPILOG() activate(t); return t->uuid

            //! faster inline creation of job
            template <typename OBJECT_POINTER,typename METHOD_POINTER> inline
            job_id enqueue(OBJECT_POINTER host, METHOD_POINTER method)
            {
                YOCTO_THREAD_ENGINE_ENQUEUE_PROLOG();
                try { new(t) task(juuid,host,method); } catch(...) { tpool.store(t); throw; }
                YOCTO_THREAD_ENGINE_ENQUEUE_EPILOG();
            }

            bool is_done(const job_id j) const throw();

            job_id  failed; //!< last failed job




        };

    }
}


#endif
