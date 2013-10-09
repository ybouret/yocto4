#ifndef YOCTO_BMOVE_INCLUDED
#define YOCTO_BMOVE_INCLUDED 1

#include "yocto/type/ints.hpp"
#include "yocto/code/unroll.hpp"


namespace yocto
{
    namespace core
    {
        
#define Y_BMOVE_FUNC(I) tgt[I] = src[I]
        template <size_t N>
        inline void bmove( void *a, const void *b ) throw()
        {
            uint8_t       *tgt = (uint8_t *)a;
            const uint8_t *src = (const uint8_t *)b;
            YOCTO_LOOP_FUNC_(N, Y_BMOVE_FUNC, 0);
        }
        
#define Y_BMOVE_IMPL(N) \
template <> inline void bmove<N>(void *a, const void *b) throw() \
{ \
typedef unsigned_int<4>::type word_t;\
*(word_t *)a = *(const word_t *)b;\
}
        Y_BMOVE_IMPL(1)
        Y_BMOVE_IMPL(2)
        Y_BMOVE_IMPL(4)
        Y_BMOVE_IMPL(8)
        
        
    }
    
    template <typename T>
    inline void bmove( T &a, const T&b ) throw()
    {
        core::bmove<sizeof(T)>(&a, &b);
    }
    
}


#endif

