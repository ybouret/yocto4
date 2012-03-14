#ifndef YOCTO_ODE_KAPS_RENTROP_INCLUDED
#define YOCTO_ODE_KAPS_RENTROP_INCLUDED 1

#include "yocto/math/ode/rosenbrock.hpp"

namespace yocto
{
    namespace math
    {
        
        namespace ode
        {
            
            template <typename T>
            class kaps_rentrop : public rosenbrock<T>
            {
            public:
                virtual ~kaps_rentrop() throw();
                explicit kaps_rentrop();
                
                typedef typename stiff_step<T>::equation equation;
                typedef typename stiff_step<T>::jacobian jacobian;
                
                virtual void operator()(array<T>       &y,
                                        array<T>       &dydx,
                                        T              &x,
                                        T               htry,
                                        T               eps,
                                        array<T>       &yscal,
                                        T              &hdid,
                                        T              &hnext,
                                        equation       &derivs,
                                        jacobian       &jacobn
                                        );
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(kaps_rentrop);
            };
            
        }
        
    }
    
}


#endif
