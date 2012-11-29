#ifndef YOCTO_THREADING_CREW_INCLUDED
#define YOCTO_THREADING_CREW_INCLUDED 1

#include "yocto/threading/condition.hpp"
#include "yocto/threading/layout.hpp"
#include "yocto/functor.hpp"

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
                const size_t rank;
                const size_t indx;
                const size_t size;
                lockable    &access;
                
                context( size_t r, size_t s, lockable &lock_ref) throw();
                ~context() throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(context);
            };
            
            typedef functor<void,TL1(context&)> task;
            
            explicit crew();
            virtual ~crew() throw();
            void run( task &sub ) throw();
            
        private:
            mutex      access; //!< shared mutex
            condition  enter;  //!< cycle synchro
            condition  leave;  //!< main thread wait on it
            size_t     ready;  //!< availability
            size_t     activ;  //!< detect end of work
            size_t     built;  //!< for terminate
            task      *proc;   //!< job to do
            bool       stop;   //!< to shutdown threads
            size_t     wlen;   //!< memory length
            void      *wksp;   //!< memory workspace
            size_t     nthr;   //!< for ctor/dtor
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(crew);
            void initialize();
            void terminate() throw();
            size_t cpu_index( size_t i, size_t n ) const throw();
            void   place();
            void   engine( size_t rank ) throw();
            
            static void launch(void *args) throw();
        };
        
    }
    
}

#endif
