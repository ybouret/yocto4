#ifndef YOCTO_ODE_ROSENBROCK_INCLUDED
#define YOCTO_ODE_ROSENBROCK_INCLUDED 1

#include "yocto/math/ode/implicit/step.hpp"
#include "yocto/math/core/lu.hpp"

namespace yocto
{
    namespace math
    {
        
        namespace ode
        {
            
            template <typename T>
            class rosenbrock : public implicit_step<T>
            {
            public:
                virtual ~rosenbrock() throw();
                void     prepare( size_t nv );
                
                
            protected:
                explicit rosenbrock();
                array<T>    &g1, &g2, &g3, &g4, &dfdx,&err,&ysav,&dysav;
                matrix<T>    dfdy;
                matrix<T>    a;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(rosenbrock);
            };

                     
        }
        
    }
    
}



#endif
