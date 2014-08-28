#ifndef YOCTO_CHEMICAL_ODE_INCLUDED
#define YOCTO_CHEMICAL_ODE_INCLUDED 1


#include "yocto/math/ode/explicit/driver-ck.hpp"

namespace yocto
{
    namespace chemical
    {
        
        typedef math::ode::driverCK<double>::type  diff_solver;
        typedef math::ode::Field<double>::Equation diff_equation;
        typedef math::ode::Field<double>::Callback diff_callback;
        
        //! u/(exp(u)-1)
        double Psi(double u) throw();
        
    }
}

#endif

