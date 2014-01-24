#ifndef YOCTO_STIFF_SOLVER_INCLUDED
#define YOCTO_STIFF_SOLVER_INCLUDED 1

#include "yocto/math/ode/implicit/step.hpp"
#include "yocto/math/ode/solver-data.hpp"

namespace yocto
{
    namespace math
    {
        namespace ode
        {
            
            template <typename T>
            class implicit_solver : public solver_data<T>
            {
            public:
                typedef typename implicit_step<T>::equation equation;
                typedef typename implicit_step<T>::jacobian jacobian;
                
                explicit implicit_solver( T user_eps );
                virtual ~implicit_solver() throw();
                
                void operator()(equation         &derivs,
                                jacobian         &jacobn,
								implicit_step<T> &forward,
								array<T>         &ystart,
								const T           x1,
								const T           x2,
								T                &h1
								);
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(implicit_solver);
            };
            
            template <
            typename T,
            template <class> class STEP
            > class implicit_driver : public implicit_solver<T>
            {
            public:
                typedef typename implicit_step<T>::equation equation;
                typedef typename implicit_step<T>::jacobian jacobian;

                
                explicit implicit_driver( T user_eps ) : implicit_solver<T>( user_eps ), step_() {}
                virtual ~implicit_driver() throw() {}
                
                inline void start( size_t nv )
                {
                    assert(nv>0);
                    this->allocate( nv ); //-- solver data
                    step_.prepare( nv );  //-- step data
                }
                
                void operator()(equation         &derivs,
                                jacobian         &jacobn,
								array<T>         &ystart,
								const T           x1,
								const T           x2,
								T                &h1
								)
                {
                    implicit_solver<T> &s = *this;
                    s(derivs,jacobn,step_,ystart,x1,x2,h1);
                }
                
            private:
                STEP<T> step_;
            };
            
            
        }
    }
}

#endif
