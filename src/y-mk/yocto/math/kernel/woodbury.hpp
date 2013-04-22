#ifndef YOCTO_MATH_KERNEL_WOODBURY_INCLUDED
#define YOCTO_MATH_KERNEL_WOODBURY_INCLUDED 1

#include "yocto/math/kernel/lu.hpp"

namespace yocto
{
    
    namespace math
    {
        //! helper to solve (A+(U.tV)) x = b
        template <typename T>
        struct woodbury
        {
            
            /**
             compute woodbury matrix from a LU matrix A
             */
            bool build(matrix<T>       &B,
                       const lu<T>     &LU,
                       const matrix<T> &A,
                       const matrix<T> &U,
                       const matrix<T> &V);
            
            
            
        };
        
        
    }
}

#endif
