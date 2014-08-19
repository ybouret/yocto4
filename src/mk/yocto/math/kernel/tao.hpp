#ifndef YOCTO_MATH_KERNEL_TAO_INCLUDED
#define YOCTO_MATH_KERNEL_TAO_INCLUDED 1

#include "yocto/math/types.hpp"
#include "yocto/type/traits.hpp"

namespace yocto
{
    namespace math
    {
        
        struct tao
        {
            template <typename ARRAY>
            static inline void set( ARRAY &a, typename ARRAY::param_type x )
            {
                for(size_t i=a.size();i>0;--i)
                {
                    a[i] = x;
                }
            }
        };
        
    }
}

#endif
