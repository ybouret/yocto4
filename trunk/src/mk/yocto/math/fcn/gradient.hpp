#ifndef YOCTO_MATH_GRADIENT_INCLUDED
#define YOCTO_MATH_GRADIENT_INCLUDED 1

#include "yocto/math/types.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
    
    namespace math
    {
        
        template <typename T>
        class gradient
        {
        public:
            explicit gradient() throw();
            virtual ~gradient() throw();
            
            //! one evaluation per component
            void compute(typename numeric<T>::scalar_field &F,
                         const array<T>                    &x,
                         const T                            Fx,
                         array<T>                          &dFdx,
                         const array<T>                    &dx);
            
            //! two evaluations per component
            void compute(typename numeric<T>::scalar_field &F,
                         const array<T>                    &x,
                         array<T>                          &dFdx,
                         const array<T>                    &dx);
            
            
        private:
            vector<T> var;
            YOCTO_DISABLE_COPY_AND_ASSIGN(gradient);
        };
    }
}

#endif
