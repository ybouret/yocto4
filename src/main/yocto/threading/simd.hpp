#ifndef YOCTO_THREADING_SIMD_INCLUDED
#define YOCTO_THREADING_SIMD_INCLUDED 1

#include "yocto/threading/threads.hpp"
#include "yocto/threading/layout.hpp"
#include "yocto/threading/condition.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
    namespace threading
    {
        
        //! Single Instruction Multiple Data framework
        class SIMD : public layout
        {
        public:

            //! context of current thread
            /**
             information about rank and size,
             and uses the vslot class to hold
             per thread information...
             */
            class Context
            {
            public:
                const size_t rank;   //!< 0..size-1
                const size_t indx;   //!< rank+1, for information
                const size_t size;   //!< the size=#threads
                lockable    &access; //!< common lock for synchronization
                virtual ~Context() throw();
                explicit Context( size_t r, size_t s, lockable &lock_ref) throw();

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Context);
            };

            class SingleContext : public faked_lock, public Context
            {
            public:
                explicit SingleContext() throw();
                virtual ~SingleContext() throw();

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(SingleContext);
            };

            typedef functor<void,TL1(Context&)> Kernel;

            
            explicit SIMD(); //!< use layout API (aka YOCTO_THREADING...)
            explicit SIMD(size_t num_threads, size_t thread_offset=0); //!< manual settings
            virtual ~SIMD() throw();


            //! execute #threads copy of kernel with different contexts
            void operator()( Kernel &K );

            //Context       & operator[](const size_t rank) throw();
            //const Context & operator[](const size_t rank) const throw();
            
            //void free()    throw();
            //void release() throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(SIMD);
            threads    workers; //!< internal threads
        public:
            mutex     &access;  //!< shared access for everyone
        private:
            condition        enter;  //!< cycle synchro
            condition        leave;  //!< main thread wait on it
            size_t           ready;  //!< availability
            size_t           activ;  //!< detect end of work
            bool             stop;   //!< to shutdown threads
            size_t           built;  //!< global count for terminate
            Kernel          *kproc;  //!< what to do during the cycle
            
            size_t           wlen; //!< extra memory size
            void            *wksp; //!< extra memory data
            
            void initialize();
            void terminate() throw();
            
            void create_contexts();
            void delete_contexts() throw();
            
            static void subroutine( void * ) throw();
            void   engine( size_t rank ) throw();
        };
    }
}

#endif
