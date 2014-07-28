#ifndef YOCTO_MATH_OPT_CGRAD_INCLUDED
#define YOCTO_MATH_OPT_CGRAD_INCLUDED 1

#include "yocto/math/types.hpp"

namespace yocto
{
    namespace math
    {
        
        template <typename T>
        struct cgrad
        {
            typedef functor<bool,TL1(const array<T>)> callback;
            
            
            //! return false only if callback returns false !
            static bool optimize(typename numeric<T>::scalar_field &func,
                                 typename numeric<T>::vector_field &grad,
                                 array<T>                          &p,
                                 const T                            ftol,
                                 callback                          *cb = 0
                                 );
        };
        
    }
}

#endif
