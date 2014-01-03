#ifndef YOCTO_MATH_ODE_EXPLICIT_RK45_INCLUDED
#define YOCTO_MATH_ODE_EXPLICIT_RK45_INCLUDED 1

#include "yocto/math/ode/explicit/step.hpp"

namespace yocto
{
    namespace math
    {
        namespace ode
        {
            template <typename T>
			class rk45 : public lw_arrays<T,ode::memory_type>
            {
            public:
                explicit rk45();
                virtual ~rk45() throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(rk45);
            };
            
            
        }
    }
}

#endif

