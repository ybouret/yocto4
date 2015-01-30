#include "yocto/utest/run.hpp"
#include "yocto/fame/integrate.hpp"
#include "yocto/fame/box.hpp"
#include "yocto/math/types.hpp"


using namespace yocto;
using namespace fame;
using namespace math;


YOCTO_UNIT_TEST_IMPL(intg)
{
    box<1,float> b1(1.5,-1.5);
    std::cerr << "b1=" << b1 << std::endl;
    array1D<double> x("x",layout1D(-10,10));
    b1.map(x);
    
    
    array1D<double> f("f", x);
    for(int i=x.lower;i<=x.upper;++i)
    {
        f[i] = Square(Cos(x[i]));
    }
    std::cerr << "x=" << x << std::endl;
    x.display(std::cerr) << std::endl;
    f.display(std::cerr) << std::endl;
    
    double ans = int1D(x,f);
    std::cerr << "ans=" << ans << std::endl;
}
YOCTO_UNIT_TEST_DONE()

