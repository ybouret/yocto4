#ifndef YOCTO_ODE_ROSENBROCK_INCLUDED
#define YOCTO_ODE_ROSENBROCK_INCLUDED 1

#include "yocto/math/ode/stiff-step.hpp"
#include "yocto/math/kernel/lu.hpp"
#include "yocto/sequence/vector.hpp"

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
                lw_array<T>    &g1, &g2, &g3, &g4, &dfdx,&err,&ysav,&dysav,&scal;
                matrix<T>      dfdy;
                matrix<T>      a;
                vector<size_t> indx;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(rosenbrock);
            };

                     
        }
        
    }
    
}



#endif
