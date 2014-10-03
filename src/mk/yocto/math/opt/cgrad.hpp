#ifndef YOCTO_MATH_OPT_CGRAD_INCLUDED
#define YOCTO_MATH_OPT_CGRAD_INCLUDED 1

#include "yocto/math/fcn/gradient.hpp"

namespace yocto
{
    namespace math
    {
        
        template <typename T>
        class cgrad : public gradient<T>
        {
        public:
            typedef functor<bool,TL1(const array<T>)> callback;
            
            
            //! return false only if callback returns false !
            /**
             this is the standalone version
             */
            static bool optimize(typename numeric<T>::scalar_field &func,
                                 typename numeric<T>::vector_field &grad,
                                 array<T>                          &p,
                                 const T                            ftol,
                                 callback                          *cb = 0
                                 );
            
            
            explicit cgrad();
            virtual ~cgrad() throw();
            
            //! return false only if callback returns false !
            bool run(typename numeric<T>::scalar_field &func,
                     array<T>                          &p,
                     const array<T>                    &dp,
                     const T                            ftol,
                     callback                          *cb = 0
                     );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(cgrad);
            typename numeric<T>::vector_field  G;
            const array<T>                    *pdp;
            
            void compute_gradient( array<T> &dFdx, const array<T> &x );
            
        };
        
        
        
    }
}

#endif
