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
                void     prepare( size_t nv );
                
                
            protected:
                explicit rosenbrock();
                lw_array<T> &g1, &g2, &g3, &g4, &dfdx,&err,&ysav,&dysav;
                matrix<T>    dfdy;
                matrix<T>    a;
                linsys<T>    lss;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(rosenbrock);
            };

                     
        }
        
    }
    
}



#endif
