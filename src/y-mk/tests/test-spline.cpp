#include "yocto/utest/run.hpp"
#include "yocto/math/dat/spline.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/types.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace math;

YOCTO_UNIT_TEST_IMPL(spline)
{
    
    const size_t nc = 10;
    const double L  = numeric<double>::two_pi;
    vector<double> X;
    vector<double> Y;
    for( size_t i=1;i<=nc;++i)
    {
        const double x = ((i-1)*L)/(nc-1);
        X.push_back(x);
        Y.push_back( Sin(x) );
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
    
    const size_t np = 200;
    
    {
        ios::ocstream fp("splines.dat",false);
        for( size_t i=0; i<=np;++i)
        {
            const double x = (i*L)/np;
            fp("%g %g %g\n", x, S1(x), S2(x));
        }
    }
    
}
YOCTO_UNIT_TEST_DONE()
