#ifndef YOCTO_MATH_FCN_DRVS_INCLUDED
#define YOCTO_MATH_FCN_DRVS_INCLUDED 1

#include "yocto/math/types.hpp"
#include "yocto/container/matrix.hpp"

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

            //! Ridder's method, return error approximate
            T diff( typename numeric<T>::function &F, T x, T h, T &err);

            //! Best effort
            T operator()( typename numeric<T>::function &F, T x, T h );
            
            
        protected:
            matrix<T> a;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(derivative);
            
        public:
            const T dtol; //!< approx minimal fractional tolerance on dFdx
            const T hopt; //!< optimal step for a scaling of x_c=1
            const T hmin; //!< should be minimal initial step
        };
        
        
        template <typename T>
        class derivative2 : public derivative<T>
        {
        public:
            explicit derivative2();
            virtual ~derivative2() throw();
            
            void diff( T &dFdx, T &d2Fdx2, typename numeric<T>::function &F, T x, const T Fx, T h, T &err);
            void operator()(T &dFdx, T &d2Fdx2, typename numeric<T>::function &F, T x, const T Fx, T h);

            
        protected:
            matrix<T> b;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(derivative2);
            
        };
        
        
    }
    
}

#endif

