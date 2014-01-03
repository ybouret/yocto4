#ifndef YOCTO_ODE_SOLVER_DATA_INCLUDED
#define YOCTO_ODE_SOLVER_DATA_INCLUDED 1


#include "yocto/sequence/lw-arrays.hpp"
#include "yocto/math/ode/types.hpp"

namespace yocto
{
    namespace math 
    {
        
        namespace ode
        {
         
            template <typename T>
            class solver_data : public lw_arrays<T,ode::memory_type>
            {
            public:
                virtual ~solver_data() throw();
                
                T hmin; //!< initial=0
				T eps;  //!< initial=ftol
				T TINY; //!< initial=1e-30, for yscal
                
            protected:
                explicit solver_data( T user_eps );
                lw_array<T> &y, &dydx, &yscal;

                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(solver_data);
            };
            
        }
        
    }
    
}

#endif
