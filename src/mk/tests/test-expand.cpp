#include "yocto/math/sig/smooth.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/ios/ocstream.hpp"
using namespace yocto;
using namespace math;


YOCTO_UNIT_TEST_IMPL(expand)
{
    double       NOISE = 0.1;
    double       dt    = 0.4;

    const size_t    n=50+alea_leq(100);
    vector<double>  x(n,0.0);
    vector<double>  y(n,0.0);
    vector<double>  z(n,0.0);
    for( size_t i=2; i <= n; ++i )
    {
        x[i] = x[i-1] + 0.5 + alea<double>();
    }

    const double fac   = (numeric<double>::two_pi / x[n]);

    for( size_t i=1; i <= n; ++i )
    {
        x[i] *= fac;
        y[i] = 0.2+sin(x[i]) + sin(3*x[i]);
        z[i] = y[i] + NOISE * ( 0.5 - alea<double>() );
    }
    y[n] = y[1];
    z[1] = y[1];
    z[n] = y[n];

    expand<double> xp_constant(expand_constant);
    expand<double> xp_cyclic(expand_cyclic);
    expand<double> xp_odd(expand_odd);
    expand<double> xp_even(expand_even);


    {
        ios::wcstream fp("xdata.dat");
        for(size_t i=1;i<=n;++i)
        {
            fp("%g %g %g\n", x[i], z[i], y[i]);
        }
    }

    {
        const unit_t  nn = n;
        ios::wcstream fp("xpand.dat");
        for(unit_t i=-nn;i<=nn+nn;++i)
        {
            fp(" %g %g", xp_constant.get_x(i, x, nn), xp_constant.get_y(i, y, nn) );
            fp(" %g %g", xp_cyclic.get_x(i, x, nn),   xp_cyclic.get_y(i, y, nn) );
            fp(" %g %g", xp_odd.get_x(i, x, nn),      xp_odd.get_y(i, y, nn) );
            fp(" %g %g", xp_even.get_x(i, x, nn),     xp_even.get_y(i, y, nn) );

            fp("\n");

        }
    }


    smooth<double> sm;
    sm.upper_range = sm.lower_range = dt/2;
    sm(xp_cyclic,1,x,z);

    {
        ios::wcstream fp("head_cyclic.dat");
        for( list< point2d<double> >::iterator i=sm.points.begin();i!=sm.points.end();++i)
        {
            const point2d<double> &p = *i;
            fp("%g %g\n", p.x, p.y);
        }
    }

    sm(xp_odd,1,x,z);
    {
        ios::wcstream fp("head_odd.dat");
        for( list< point2d<double> >::iterator i=sm.points.begin();i!=sm.points.end();++i)
        {
            const point2d<double> &p = *i;
            fp("%g %g\n", p.x, p.y);
        }
    }

}
YOCTO_UNIT_TEST_DONE()
