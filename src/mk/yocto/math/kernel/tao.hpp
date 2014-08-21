#ifndef YOCTO_MATH_KERNEL_TAO_INCLUDED
#define YOCTO_MATH_KERNEL_TAO_INCLUDED 1

#include "yocto/math/types.hpp"
#include "yocto/type/traits.hpp"
#include "yocto/math/kernel/matrix.hpp"
#include "yocto/code/unroll.hpp"

namespace yocto
{
    namespace math
    {
        
        struct tao
        {
            
#define YOCTO_TAO_LOOP(N,FUNC) YOCTO_LOOP_FUNC(N,Y_TAO_##FUNC,1)
            
            template <typename ARRAY>
            static inline void ld( ARRAY &a, typename ARRAY::param_type x ) throw()
            {
#define Y_TAO_LD(I) a[I] = x
                YOCTO_TAO_LOOP(a.size(),LD);
            }
            
            template <typename ARR, typename BRR>
            static inline void set( ARR &a, const BRR &b ) throw()
            {
                assert(a.size()<=b.size());
#define Y_TAO_SET(I) a[I] = b[I]
                YOCTO_TAO_LOOP(a.size(),SET);
            }
            
            template <typename ARR, typename BRR>
            static inline void neg( ARR &a, const BRR &b ) throw()
            {
                assert(a.size()<=b.size());
#define Y_TAO_NEG(I) a[I] = -static_cast<typename ARR::type>(b[I])
                YOCTO_TAO_LOOP(a.size(),NEG);
            }

            
            template<typename ARR, typename BRR>
            static inline void mulset( ARR &a, typename ARR::param_type x, const BRR &b ) throw()
            {
                assert(a.size()<=b.size());
#define Y_TAO_MULSET(I) a[I] = x*b[I]
                YOCTO_TAO_LOOP(a.size(),MULSET);
            }
            
            template<typename ARR, typename BRR>
            static inline void muladd( ARR &a, typename ARR::param_type x, const BRR &b ) throw()
            {
                assert(a.size()<=b.size());
#define Y_TAO_MULADD(I) a[I] += x*b[I]
                YOCTO_TAO_LOOP(a.size(),MULADD);
            }
            
            template<typename ARR, typename BRR>
            static inline void mulsub( ARR &a, typename ARR::param_type x, const BRR &b ) throw()
            {
                assert(a.size()<=b.size());
#define Y_TAO_MULSUB(I) a[I] -= x*b[I]
                YOCTO_TAO_LOOP(a.size(),MULSUB);
            }
            
        };
        
    }
}

#endif
