#ifndef YOCTO_THREADING_CREW_INCLUDED
#define YOCTO_THREADING_CREW_INCLUDED 1

#include "yocto/threading/threads.hpp"
#include "yocto/threading/layout.hpp"
#include "yocto/container/slots.hpp"
#include "yocto/threading/condition.hpp"

namespace yocto
{
    namespace threading
    {
        class crew : public layout
        {
        public:
            class context
            {
            public:
                context(const size_t    the_rank,
                        const size_t    the_size,
                        lockable       &the_lock) throw();
                ~context() throw();
                
                const size_t rank;   //!< 0-size-1
                const size_t size;   //!< how many contextes
                lockable    &access; //!< shared access
                const size_t indx;   //!< 1-size
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(context);
                friend class crew;
                void        *priv;
            };

            class single_context : public faked_lock, public context
            {
            public:
                single_context() throw();
                virtual ~single_context() throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(single_context);
            };


            explicit crew();
            explicit crew(const size_t num_cpus, const size_t cpu_start=0);
            virtual ~crew() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(crew);
            threads           workers; //!< all the threads
        public:
            mutex            &access;  //!< shared access

        private:
            size_t            ready;    //!< for synchronyzation
            condition         cycle;    //!< waiting for cycle
            condition         synch;    //!< waiting for synch
            slots_of<context> contexts;
            const bool        dying;

            void init();
            void quit() throw();


            static void worker_call(void *) throw();
            void        worker_loop(const context &ctx) throw();
        };
    };
}

#endif

