#include "yocto/utest/run.hpp"
#include "yocto/math/dat/extrapolate.hpp"
#include "yocto/math/types.hpp"

#include "yocto/sequence/vector.hpp"

#include "yocto/ios/ocstream.hpp"


using namespace yocto;
using namespace math;


YOCTO_UNIT_TEST_IMPL(extr)
{
    vector<double> x;
    vector<double> y;
    
    ios::ocstream fp("pade.dat", false);
    for(size_t i=1;i<=8;++i)
    {
        double xx = 0.1/i;
        double yy = Cos(5*xx);
        x.push_back(xx);
        y.push_back(yy);
        
        const double z = extrapolate<double>::zpade(x, y);
        std::cerr << "z=" << z << std::endl;
        fp("%g %g\n", xx, yy);
    }
    
    
}
YOCTO_UNIT_TEST_DONE()
