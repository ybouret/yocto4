#include "yocto/threading/simd.hpp"

namespace yocto
{
    namespace threading
    {
        
#define YOCTO_SIMD_CTOR() \
workers( "SIMD" ), \
access( workers.access )
        
        SIMD:: SIMD() :
        layout(),
        YOCTO_SIMD_CTOR()
        {
            initialize();
        }
        
        SIMD:: SIMD( size_t num_threads, size_t thread_offset ) :
        layout( num_threads, thread_offset),
        YOCTO_SIMD_CTOR()
        {
            initialize();
        }
        
        void SIMD:: initialize()
        {
            workers.reserve(size);
        }
    }
}
