#ifndef YOCTO_MATH_FCN_BESSEL_INCLUDED
#define YOCTO_MATH_FCN_BESSEL_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
    
    namespace math
    {
        
        template <typename T>
        struct bessel
        {
            static T j0(T x); //!< any x
            static T y0(T x); //!< x > 0
        };
        
    }
    
}

#endif
