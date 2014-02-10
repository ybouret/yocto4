#ifndef YOCTO_THREADING_SIMD_INCLUDED
#define YOCTO_THREADING_SIMD_INCLUDED 1

#include "yocto/threading/threads.hpp"
#include "yocto/threading/layout.hpp"
#include "yocto/threading/context.hpp"
#include "yocto/threading/condition.hpp"

namespace yocto
{
    namespace threading
    {
        
        class SIMD : public layout
        {
        public:
            explicit SIMD(); //!< use layout
            explicit SIMD(size_t num_threads, size_t thread_offset=0); //!< use layout
            virtual ~SIMD() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(SIMD);
            threads    workers;
            mutex     &access;
            condition  enter;  //!< cycle synchro
            condition  leave;  //!< main thread wait on it
            size_t     ready;  //!< availability
            size_t     activ;  //!< detect end of work
            bool       stop;   //!< to shutdown threads
            
            size_t   wlen; //!< extra memory size
            void    *wksp; //!< extra memory data
        
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
