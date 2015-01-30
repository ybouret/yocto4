#include "yocto/chemical/ode.hpp"
#include <cmath>

namespace yocto
{
    namespace chemical
    {
        
        double GHK_Psi(double x) throw()
        {
            if(fabs(x)>=0.1)
            {
                return x/(exp(x)-1.0);
            }
            else
            {
                const double x2  = x*x;
                const double x4  = x2*x2;
                const double x6  = x2 * x4;
                const double x8  = x4*x4;
                const double x10 = x4*x6;
                const double x9  = x8*x;
                const double num = 1.0 - x/2.0 + x2/12.0 - x4/720.0 + x6/30240.0;
                const double den = 1.0 + x8/1209600.0 + x9/2419200.0 + x10/8553600.0;
                return num/den;
            }
        }
        
    }
}

