#ifndef YOCTO_THREADING_SIMD_INCLUDED
#define YOCTO_THREADING_SIMD_INCLUDED 1

#include "yocto/threading/threads.hpp"
#include "yocto/threading/layout.hpp"

namespace yocto
{
    namespace threading
    {
        
        class SIMD : public layout
        {
        public:
            explicit SIMD(); //!< use layout
            explicit SIMD(size_t num_threads, size_t thread_offset=0); //!< use layout
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(SIMD);
            threads  workers;
            mutex   &access;
            
            void initialize();
            
            static void engine( void * ) throw();
            
        };
    }
}

#endif
