#ifndef YOCTO_STIFF_SOLVER_INCLUDED
#define YOCTO_STIFF_SOLVER_INCLUDED 1

#include "yocto/math/ode/stiff-step.hpp"
#include "yocto/math/ode/solver-data.hpp"

namespace yocto
{
    namespace math 
    {
        namespace ode
        {
            
            template <typename T>
            class stiff_solver : public solver_data<T>
            {
            public:
                typedef typename stiff_step<T>::equation equation;
                typedef typename stiff_step<T>::jacobian jacobian;
                
                explicit stiff_solver();
                virtual ~stiff_solver() throw();
                
                void operator()(equation         &derivs,
                                jacobian         &jacobn,
								stiff_step<T>    &forward,
								array<T>         &ystart,
								const T           x1,
								const T           x2,
								T                &h1
								);
                          
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(stiff_solver);
            };
            
        }
    }
}

#endif
