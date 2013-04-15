#include "yocto/utest/run.hpp"
#include "yocto/math/v2d.hpp"
#include "yocto/math/types.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace math;

YOCTO_UNIT_TEST_IMPL(geom)
{
  
    ios::ocstream fp("angle.dat", false);
    for( double a=0; a <= 2*numeric<double>::two_pi;a += 0.02)
    {
        v2d<double> v( Cos(a), Sin(a) );
        const double b = v.angle();
        const double c = b < 0 ? b + numeric<double>::two_pi : b;
        fp("%g %g %g\n", a, b, c);
    }
    
}
YOCTO_UNIT_TEST_DONE()
