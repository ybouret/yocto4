#ifndef YOCTO_ODE_ROSENBROCK_INCLUDED
#define YOCTO_ODE_ROSENBROCK_INCLUDED 1

#include "yocto/math/ode/stiff-step.hpp"
#include "yocto/math/kernel/linsys.hpp"

namespace yocto
{
    namespace math
    {
        
        namespace ode
        {
            
            template <typename T>
            class rosenbrock : public stiff_step<T>
            {
            public:
                virtual ~rosenbrock() throw();
                
            protected:
                explicit rosenbrock();
                lw_array<T> &g1, &g2, &g3, &g4, &dfdx,&err,&ysav,&dysav;
                linsys<T>    lss;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(rosenbrock);
            };

            template <typename T>
            class rosenbrock_shampine : public rosenbrock<T>
            {
            public:
                virtual ~rosenbrock_shampine() throw();
                explicit rosenbrock_shampine();
                                
                typedef typename stiff_step<T>::equation equation;
                typedef typename stiff_step<T>::jacobian jacobian;

                virtual void operator()(array<T>       &y,
                                        const array<T> &dydx,
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
                YOCTO_DISABLE_COPY_AND_ASSIGN(rosenbrock_shampine);
            };
            
        }
        
    }
    
}



#endif
