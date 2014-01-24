#ifndef YOCTO_MATH_ODE_EXPLICIT_CONTROLER_INCLUDED
#define YOCTO_MATH_ODE_EXPLICIT_CONTROLER_INCLUDED 1

#include "yocto/math/ode/explicit/step.hpp"

namespace yocto
{
    namespace math
    {
        namespace ode
        {
            //! adaptive controler
            /**
             needs to be prepared before integration
             */
            template <typename T>
            class explicit_controler : public Field<T>::Arrays
            {
            public:
                typedef typename Field<T>::Equation equation;
                typedef typename Field<T>::Callback callback;
                
                virtual ~explicit_controler() throw();
                
                //! try to forward
                /**
                 \param forward an explicit step (RKCK,RKDP)
                 \param y       input/output values
                 \param drvs    the differential equation
                 \param x       input/output coordinate
                 \param h_try   trial step
                 \param h_did   final step
                 \param h_next  predicted net step
                 \param yscal   scaling values
                 \param eps     fractional tolerance for h
                 */
                virtual void operator()(explicit_step<T> &forward,
                                        array<T>         &y,
                                        const array<T>   &dydx,
                                        equation         &drvs,
                                        T                &x,
                                        const T           h_try,
                                        T                &h_did,
                                        T                &h_next,
                                        const array<T>   &yscal,
                                        const T           eps,
                                        callback         *cb) = 0;
            protected:
                explicit explicit_controler(size_t na);
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(explicit_controler);
            };
        }
    }
}

#endif
