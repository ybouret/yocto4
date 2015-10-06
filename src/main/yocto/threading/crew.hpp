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

        //! a context = range information for a working thread
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

        //! a threading kernel, using a context
        typedef functor<void,TL1(context&)> kernel;

        //! interface to execute a kernel
        class kernel_executor : public object
        {
        public:
            const size_t failure;

            virtual ~kernel_executor() throw();

            virtual void   operator()(kernel &) throw() = 0;
            virtual size_t num_threads()  const throw() = 0;
            
            template <typename OBJECT_POINTER,typename METHOD_POINTER>
            inline void call( OBJECT_POINTER h, METHOD_POINTER m )
            {
                kernel_executor   &self = *this;
                kernel  k(h,m);
                self(k);
            }

        protected:
            explicit kernel_executor() throw();

            void no_failure() const throw();
            void set_failure(size_t) const throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(kernel_executor);
        };

        //! sequential executor, for debugging or single thread run
        class sequential_executor :
        public faked_lock,
        public context,
        public kernel_executor
        {
        public:
            virtual ~sequential_executor() throw();
            explicit sequential_executor() throw();
            virtual void  operator()(kernel &) throw();
            virtual size_t num_threads() const throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(sequential_executor);
        };

        class crew : public layout, public kernel_executor
        {
        public:
            explicit crew(bool setVerbose);
            explicit crew(const size_t num_cpus, const size_t cpu_start,bool setVerbose);
            virtual ~crew() throw();

            virtual void operator()(kernel &k) throw();
            virtual size_t num_threads() const throw();

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

