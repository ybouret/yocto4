#include "yocto/utest/run.hpp"
#include "yocto/math/dat/spline.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/types.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/math/kernel/matrix.hpp"

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
    
    matrix<double> Y2(5,nc);
    const double width = X[nc] - X[1];
    std::cerr << "spline natural..." << std::endl;
    spline<double>::compute(spline_natural,       X, Y, Y2[1]);
    
    std::cerr << "spline periodic..." << std::endl;
    spline<double>::compute(spline_periodic,      X, Y, Y2[2]);
    
    std::cerr << "spline tangent left..." << std::endl;
    spline<double>::compute(spline_tangent_left,  X, Y, Y2[3],0,0);
    
    std::cerr << "spline tangent right..." << std::endl;
    spline<double>::compute(spline_tangent_right, X, Y, Y2[4],0,0);
    
    std::cerr << "spline tangent both..." << std::endl;
    spline<double>::compute(spline_tangent_both,  X, Y, Y2[5],0,0);

    
    spline1D<double> S1(spline_natural,X,Y);
    spline1D<double> S2(spline_periodic,X,Y);
    spline1D<double> S3(spline_tangent_left,X,Y,0,0);
    spline1D<double> S4(spline_tangent_right,X,Y,0,0);
    spline1D<double> S5(spline_tangent_both,X,Y,0,0);

    
    const size_t np = 200;
    
    {
        ios::ocstream fp("splines.dat",false);
        for( size_t i=0; i<=np;++i)
        {
            const double x = (i*L)/np;
            fp("%g", x);
            fp(" %g", spline<double>::eval(x, X, Y, Y2[1], NULL));
            fp(" %g", spline<double>::eval(x, X, Y, Y2[2], &width));
            fp(" %g", spline<double>::eval(x, X, Y, Y2[3], NULL));
            fp(" %g", spline<double>::eval(x, X, Y, Y2[4], NULL));
            fp(" %g", spline<double>::eval(x, X, Y, Y2[5], NULL));
            fp("\n");
        }
    }
    
    {
        ios::ocstream fp("splines_bis.dat",false);
        for( size_t i=0; i<=np;++i)
        {
            const double x = (i*L)/np;
            fp("%g", x);
            fp(" %g", S1(x));
            fp(" %g", S2(x));
            fp(" %g", S3(x));
            fp(" %g", S4(x));
            fp(" %g", S5(x));
            fp("\n");
        }
        
    }
    
    
}
YOCTO_UNIT_TEST_DONE()
