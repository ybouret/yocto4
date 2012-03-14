#ifndef YOCTO_ODE_STIFF_STEP_INCLUDED
#define YOCTO_ODE_STIFF_STEP_INCLUDED 1

#include "yocto/math/ode/types.hpp"
#include "yocto/sequence/lw-arrays.hpp"
#include "yocto/math/kernel/matrix.hpp"

namespace yocto
{
    namespace math
    {
        
        namespace ode
        {
            
            template <typename T>
            class stiff_step : public lw_arrays<T,memory_type>
            {
            public:
                typedef typename field<T>::type          equation;
                typedef typename field<T>::jacobian_type jacobian;
                
                virtual ~stiff_step() throw();
                
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
            protected:
                explicit stiff_step( const size_t num );
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(stiff_step);
            };
            
        }
        
    }
}



#endif
