#include "yocto/utest/run.hpp"
#include "yocto/math/dat/trigonometric.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/code/ipower.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;
using namespace math;


YOCTO_UNIT_TEST_IMPL(trigo)
{
    // make sample
    const size_t   n = 20 + alea_leq(10);
    vector<double> x(n,0);
    double period = 0;
    for( size_t i=1; i <=n; ++i )
    {
        x[i] = period;
        period += alea<double>();
    }
    
    std::cerr << "on " << n << " points" << std::endl;
    const double tfac = numeric<double>::two_pi / period;
    vector<double> y(n,0);
    {
        ios::ocstream fp("trig0.dat",false);
        for( size_t i=1; i<=n; ++i )
        {
            x[i] *= tfac;
            y[i]  = ipower(Cos(2*x[i]), 3);
            fp("%g %g\n",x[i],y[i]);
        }
    }
    
    // prepare trigo interp
    LU<double>            solver;
    trigonometric<double> trig(x,solver);
    vector<double>        coef(y);
    trig.compute(coef);
    {
        ios::ocstream fp("trig1.dat",false);
        const size_t m = 400 + alea_leq(400);
        for( size_t i=0; i <= m; ++i )
        {
            const double theta = (i*numeric<double>::two_pi) / m;
            fp("%g %g\n", theta, trig(theta,coef));
        }
    }
    
}
YOCTO_UNIT_TEST_DONE()

