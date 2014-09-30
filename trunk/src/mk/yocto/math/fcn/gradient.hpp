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
            static void compute(scalar_field    &F,
                                array<T>        &x,
                                const T          Fx,
                                array<T>        &dFdx,
                                const array<T>  &dx);
            
            //! two evaluations per component
            static void compute(scalar_field   &F,
                                array<T>       &x,
                                array<T>       &dFdx,
                                const array<T> &dx);
            
            
            
            void operator()(scalar_field   &F,
                            array<T>       &x,
                            array<T>       &dFdx,
                            const array<T> &dx);
            
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(gradient);
            typedef typename numeric<T>::function scalar_function;
            scalar_field   *pF;
            size_t          ix;
            array<T>       *px;
            scalar_function gf;
            
            T eval(T u);
            
        };
    }
}

#endif
