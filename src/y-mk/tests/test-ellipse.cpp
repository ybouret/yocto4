#include "yocto/utest/run.hpp"
#include "yocto/math/alg/ellipse.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/math/types.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/string/conv.hpp"

using namespace yocto;
using namespace math;


YOCTO_UNIT_TEST_IMPL(ellipse)
{
    size_t n = 10;
    if(argc>1)
        n = strconv::to<size_t>(argv[1],"n");
    
    ellipse<double> ell;
    
    const double a = 4 + 2*alea<double>();
    const double b = 1.5 + alea<double>();
    const double phi =  alea<double>() * numeric<double>::two_pi;
    const double CosPhi = Cos(phi);
    const double SinPhi = Sin(phi);
    
    {
        ios::ocstream fp("ell.dat", false);
        
        for( size_t i=1; i <= n; ++i )
        {
            const double theta = alea<double>() * numeric<double>::two_pi;
            const double X     = a * Cos(theta);
            const double Y     = b * Sin(theta);
            const double x     = 0.2 + X*CosPhi - Y*SinPhi;
            const double y     = 0.1 + X*SinPhi + Y*CosPhi;
            fp("%g %g\n", x, y);
            ell.append(x,y);
        }
    }
    std::cerr << "S=" << ell.__S() << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
