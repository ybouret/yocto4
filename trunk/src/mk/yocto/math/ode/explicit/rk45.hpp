#ifndef YOCTO_MATH_ODE_EXPLICIT_RK45_INCLUDED
#define YOCTO_MATH_ODE_EXPLICIT_RK45_INCLUDED 1

#include "yocto/math/ode/explicit/controler.hpp"

namespace yocto
{
    namespace math
    {
        namespace ode
        {
            template <typename T>
			class RK45 : public explicit_controler<T>
            {
            public:
                typedef typename Field<T>::Equation equation;
                typedef typename Field<T>::Callback callback;
                
                explicit RK45();
                virtual ~RK45() throw();
                
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
                void operator()(explicit_step<T> &forward,
                                array<T>         &y,
                                const array<T>   &dydx,
                                equation         &drvs,
                                T                &x,
                                const T           h_try,
                                T                &h_did,
                                T                &h_next,
                                const array<T>   &yscal,
                                const T           eps,
                                callback         *cb);
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(RK45);
                array<T> &yerr,&ytmp;
            };
            
            
        }
    }
}

#endif

