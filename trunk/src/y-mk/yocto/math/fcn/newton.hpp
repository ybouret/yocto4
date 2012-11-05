#ifndef YOCTO_MATH_NEWTON_INCLUDED
#define YOCTO_MATH_NEWTON_INCLUDED 1

#include "yocto/math/types.hpp"
#include "yocto/math/fcn/jacobian.hpp"

namespace yocto
{
    
    namespace math
    {
        
        template <typename T>
        struct Newton
        {
            typedef typename numeric<T>::vector_field           Function;
            typedef typename jacobian<T>::type                  Jacobian;
            
            //! solve the function using hybrid Newton-Raphson/Conjugated Gradient
            /**
             The functions need to be scaled.
             TODO: detect other spurious points...
             */
            static bool solve( Function &func, Jacobian &jac, array<T> &X, T ftol );
            static bool qsolve( Function &func, Jacobian &jac, array<T> &X, T ftol );

            
        };
        
    }
}

#endif

