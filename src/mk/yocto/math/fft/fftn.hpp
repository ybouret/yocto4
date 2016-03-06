#ifndef YOCTO_MATH_FFTN_INCLUDED
#define YOCTO_MATH_FFTN_INCLUDED 1

#include "yocto/math/complex.hpp"


namespace yocto
{
    
    namespace math
    {

        template <typename T>
        struct FourierN
        {
            //! generic fourier
            /**
             nn[1]..nn[ndim]
             */
            static void Compute(T *data, const size_t nn[], const size_t ndim, int isign) throw();
            
        };
        
    }

}

#endif
