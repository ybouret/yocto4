#include "yocto/utest/run.hpp"
#include "yocto/math/fcn/gradient.hpp"


using namespace yocto;
using namespace math;

namespace
{
    static inline double F(const array<double> &var)
    {
        const double x=var[1];
        const double y=var[2];
        const double t=0.5*(x+y);
        const double u=0.6*(x-y);
        return 1+2*t*t+3*u*u;
    }
}


YOCTO_UNIT_TEST_IMPL(grad)
{
    double (*proc)(const array<double> &) = F;
    numeric<double>::scalar_field FF(proc);
    
    const size_t n = 2;
    vector<double> var(n,0);
    vector<double> G(n,0);
    vector<double> dvar(n,1e-4);
    gradient<double> grad;
    matrix<double>   H(n,n);
    
    var[1] = 0.1;
    var[2] = -0.2;
    
    grad.compute(FF, var, G, dvar);
    std::cerr << "grad="  << G << std::endl;
    
    grad.compute(FF, var, FF(var), G, dvar, H);
    std::cerr << "grad=" << G << std::endl;
    std::cerr << "hess=" << H << std::endl;
    
    
    
}
YOCTO_UNIT_TEST_DONE()

