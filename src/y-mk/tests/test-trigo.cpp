#include "yocto/utest/run.hpp"
#include "yocto/math/dat/trigonometric.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/code/ipower.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace math;


YOCTO_UNIT_TEST_IMPL(trigo)
{
    // make sample
    const size_t   n = 10 + alea_leq(10);
    vector<double> x(n,0);
    double period = 0;
    for( size_t i=1; i <=n; ++i )
    {
        x[i] = period;
        period += alea<double>();
    }
    
    const double tfac = numeric<double>::two_pi / period;
    vector<double> y(n,0);
    {
        ios::ocstream fp("trig0.dat",false);
        for( size_t i=1; i<=n; ++i )
        {
            x[i] *= tfac;
            y[i]  = ipower(Cos(x[i]), 3);
            fp("%g %g\n",x[i],y[i]);
        }
    }
    
    // prepare trigo interp
    linsys<double>        solver;
    trigonometric<double> trig(x,solver);
    vector<double>        coef(y);
    trig.compute(coef, solver);
    {
        ios::ocstream fp("trig1.dat",false);
        const size_t m = 50 + alea_leq(50);
        for( size_t i=0; i <= m; ++i )
        {
            const double theta = (i*numeric<double>::two_pi) / m;
            fp("%g %g\n", theta, trig(theta,coef));
        }
    }
    
}
YOCTO_UNIT_TEST_DONE()

