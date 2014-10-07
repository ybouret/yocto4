#ifndef YOCTO_MATH_GRADIENT_INCLUDED
#define YOCTO_MATH_GRADIENT_INCLUDED 1

#include "yocto/math/fcn/drvs.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
    
    namespace math
    {
        
        template <typename T>
        class gradient : public derivative<T>
        {
        public:
            typedef typename numeric<T>::scalar_field scalar_field;
            
            explicit gradient();
            
            virtual ~gradient() throw();
            
            //! one evaluation per component
            static void compute1(scalar_field    &F,
                                 array<T>        &x,
                                 const T          Fx,
                                 array<T>        &dFdx,
                                 const array<T>  &dx);
            
            //! two evaluations per component
            static void compute2(scalar_field   &F,
                                 array<T>       &x,
                                 array<T>       &dFdx,
                                 const array<T> &dx);
            
            
            //! using partial derivatives
            void compute(scalar_field   &F,
                         array<T>       &x,
                         array<T>       &dFdx,
                         const array<T> &dx);
            
            
        protected:
            scalar_field   *pF;
            
            //! using pF
            void __compute(array<T>       &x,
                           array<T>       &dFdx,
                           const array<T> &dx);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(gradient);
            typedef typename numeric<T>::function scalar_function;
            size_t          ix; //used by compute
            array<T>       *px; //used by compute
            scalar_function gf; // (this, & eval )
            
            T eval(T u);
            
        };
    }
}

#endif
