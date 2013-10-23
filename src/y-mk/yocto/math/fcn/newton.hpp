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
            typedef typename numeric<T>::vector_field                    Function;
            typedef typename jacobian<T>::type                           Jacobian;
            typedef functor<bool,TL2(const array<T> &,const array<T> &)> Callback; //! Callback(F,X)
            
            //! solve the function using hybrid Newton-Raphson/Conjugated Gradient
            /**
             The functions need to be scaled.
             The inversion of the Jacobian is  performed by SVD
             so that the method switch to CJ when an ill-conditioned matrix is met.
             */
            
            static bool solve( Function &func, Jacobian &jac, array<T> &X, T ftol, Callback *cb = 0 );
            
        };
        
    }
}

#endif

