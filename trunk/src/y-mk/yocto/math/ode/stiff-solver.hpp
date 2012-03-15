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
         
            template <
            typename T,
            template <class> class STEP
            > class stiff_driver : public stiff_solver<T>
            {
            public:
                typedef typename stiff_step<T>::equation equation;
                typedef typename stiff_step<T>::jacobian jacobian;
                
                explicit stiff_driver() : stiff_solver<T>(), step_() {}
                virtual ~stiff_driver() throw() {}
                
                inline void start( size_t nv ) 
                {
                    assert(nv>0);
                    this->prepare( nv );
                    step_.prepare( nv );
                }
                
                void operator()(equation         &derivs,
                                jacobian         &jacobn,
								array<T>         &ystart,
								const T           x1,
								const T           x2,
								T                &h1
								)
                {
                    stiff_solver<T> &s = *this;
                    s(derivs,jacobn,step_,ystart,x1,x2,h1);
                }
                
            private:
                STEP<T> step_;
            };
            
            
        }
    }
}

#endif
