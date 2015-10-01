#ifndef YOCTO_THREADING_CREW_INCLUDED
#define YOCTO_THREADING_CREW_INCLUDED 1

#include "yocto/threading/threads.hpp"
#include "yocto/threading/layout.hpp"
#include "yocto/sequence/slots.hpp"
#include "yocto/threading/condition.hpp"
#include "yocto/functor.hpp"
#include "yocto/parallel/basic.hpp"

namespace yocto
{
    namespace threading
    {
        class crew : public layout
        {
        public:

            //! a context = range information for a worker
            class context
            {
            public:
                context(const size_t    the_rank,
                        const size_t    the_size,
                        lockable       &the_lock) throw();
                virtual ~context() throw();
                
                const size_t rank;   //!< 0-size-1
                const size_t size;   //!< how many contextes
                lockable    &access; //!< shared access
                const size_t indx;   //!< 1-size

                template <typename T> inline
                void split(T &offset,T &length) const throw()
                {
                    parallel::basic_split<T>(rank, size, offset, length);
                }


            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(context);
                friend class crew;
                void        *priv;
            };

            //! for testing sequential code
            class single_context : public faked_lock, public context
            {
            public:
                single_context() throw();
                virtual ~single_context() throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(single_context);
            };


            explicit crew(bool setVerbose=false);
            explicit crew(const size_t num_cpus, const size_t cpu_start,bool setVerbose=false);
            virtual ~crew() throw();


            typedef functor<void,TL1(context&)> kernel;
            
            void operator()(kernel &k) throw();

            template <typename OBJECT_POINTER,typename METHOD_POINTER>
            void operator()( OBJECT_POINTER h, METHOD_POINTER m )
            {
                crew   &self = *this;
                kernel  k(h,m);
                self(k);
            }



        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(crew);
            threads           workers; //!< all the threads
        public:
            mutex            &access;  //!< shared access

        private:
            size_t            ready;    //!< for synchronyzation
            const bool        dying;    //!< flag for dying...
            kernel           *kproc;    //!< what to do for the current cycle
            condition         cycle;    //!< waiting for cycle
            condition         synch;    //!< waiting for synch
            slots_of<context> contexts;

            void init();
            void quit() throw();


            static void worker_call(void *) throw();
            void        worker_loop(context &ctx) throw();

        public:
            const size_t failure; //!< set to rank+1 if failure
        };
    };
}

#endif

