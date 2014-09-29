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
        return (x+y)/(1.0+Square(Cos(x))*Square(Cos(y)));
    }
}


YOCTO_UNIT_TEST_IMPL(grad)
{
    double (*proc)(const array<double> &) = F;
    numeric<double>::scalar_field FF(proc);
    
    vector<double> var(2,0);
    vector<double> G(2,0);
    vector<double> dvar(2,1e-4);
    gradient<double> grad;
    grad.compute(FF, var, G, dvar);
    
    std::cerr << "grad="  << G << std::endl;
    
    
    
}
YOCTO_UNIT_TEST_DONE()

