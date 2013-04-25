#include "yocto/utest/run.hpp"
#include "yocto/math/dat/spline.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/types.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace math;

YOCTO_UNIT_TEST_IMPL(spline)
{
    
    const size_t nc = 10;
    const double L  = numeric<double>::two_pi;
    vector<double> X(nc,0);
    vector<double> Y(nc,0);
    for(size_t i=2;i<=nc;++i)
    {
        X[i] = X[i-1] + (0.01+alea<double>());
    }
    
    {
        const double fac = L / X[nc];
        for(size_t i=2;i<=nc;++i)
        {
            X[i] *= fac;
            Y[i]  = Sin(X[i]);
        }
    }
    
    
    {
        ios::ocstream fp("origin.dat",false);
        for(size_t i=1; i <= X.size(); ++i)
        {
            fp("%g %g\n", X[i], Y[i]);
        }
    }
    
    const spline<double>::boundaries bnd1( spline_regular );
    spline<double> S1(X,Y,bnd1);
    
    const spline<double>::boundaries bnd2(false,0,false,0);
    spline<double> S2(X,Y,bnd2);
    
    const spline<double>::boundaries bnd3( spline_cyclic );
    spline<double> S3(X,Y,bnd3);
    
    
    const size_t np = 200;
    
    {
        ios::ocstream fp("splines.dat",false);
        for( size_t i=0; i<=np;++i)
        {
            const double x = (i*L)/np;
            fp("%g %g %g %g\n", x, S1(x), S2(x), S3(x));
        }
    }
    
}
YOCTO_UNIT_TEST_DONE()
