#ifndef YOCTO_MATH_KERNEL_WOODBURY_INCLUDED
#define YOCTO_MATH_KERNEL_WOODBURY_INCLUDED 1

#include "yocto/math/kernel/lu.hpp"
#include "yocto/math/kernel/tridiag.hpp"

namespace yocto
{
    
    namespace math
    {
        //! helper to solve (A+(U.tV)) x = b
        /**
         Each routine return a matix B.
         First solve A.y = b.
         Then        x   = B.y
         */
        template <typename T>
        struct woodbury
        {
            
            /**
             compute woodbury matrix from a LU matrix A
             */
            static bool build(matrix<T>       &B,
                              const lu<T>     &LU,
                              const matrix<T> &A,
                              const matrix<T> &U,
                              const matrix<T> &V);
            
            static bool build(matrix<T>        &B,
                              const xtridiag<T>&A,
                              const matrix<T>  &U,
                              const matrix<T>  &V);
            
            
        };
        
        
    }
}

#endif
