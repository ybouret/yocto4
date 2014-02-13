#ifndef YOCTO_MATH_FCN_DRVS_INCLUDED
#define YOCTO_MATH_FCN_DRVS_INCLUDED 1

#include "yocto/math/types.hpp"
#include "yocto/math/kernel/matrix.hpp"

namespace yocto
{
    namespace math
    {
     
        template <typename T>
        class derivative
        {
        public:
            explicit derivative();
            virtual ~derivative() throw();
            
            T diff( typename numeric<T>::function &F, T x, T h, T &err);
            
            T operator()( typename numeric<T>::function &F, T x, T h );
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(derivative);
            matrix<T> a;
            
        public:
            const T dtol; //!< approx minimal fractional tolerance on dFdx
            const T hopt; //!< optimal step for a scaling of x_c=1
            const T hmin; //!< should be minimal initial step
        };
        
        
    }
    
}

#endif

