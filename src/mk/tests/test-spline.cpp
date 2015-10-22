#include "yocto/utest/run.hpp"
#include "yocto/math/dat/spline.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/types.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/container/matrix.hpp"

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
        ios::ocstream fp("derivs.dat",false);
        matrix<double> Y1(5,nc);
        for(size_t i=1; i <= 5; ++i)
        {
            spline<double>::derivs(Y1[i], X, Y, Y2[i]);
        }
        
        for(size_t i=1; i <= nc; ++i )
        {
            fp("%g", X[i]);
            for(size_t j=1; j<=5;++j)
            {
                fp(" %g", Y1[j][i]);
            }
            fp("\n");
        }
    }
    
    {
        ios::ocstream fp("derivs2.dat",false);
        for(size_t i=1; i <= nc; ++i )
        {
            fp("%g", X[i]);
            for(size_t j=1; j<=5;++j)
            {
                fp(" %g", Y2[j][i]);
            }
            fp("\n");
        }
    }
    
    
    spline1D<double> S1(spline_natural,      X,Y);
    spline1D<double> S2(spline_periodic,     X,Y);
    spline1D<double> S3(spline_tangent_left, X,Y,0,0);
    spline1D<double> S4(spline_tangent_right,X,Y,0,0);
    spline1D<double> S5(spline_tangent_both, X,Y,0,0);
    
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

#include "yocto/string/conv.hpp"

typedef v2d<double> vtx;
#include "yocto/sequence/list.hpp"

YOCTO_UNIT_TEST_IMPL(spline2d)
{
    size_t nc = 4;
    if(argc>1) nc = strconv::to<size_t>(argv[1],"nc");
    if(nc<3) throw exception("not enough points");
    
    vector<double> t(nc,0);
    vector<vtx>    points(nc+1,as_capacity);
    vector<double> theta(nc,0);
    
    //--------------------------------------------------------------------------
    // construct angles
    //--------------------------------------------------------------------------
    for(size_t i=2;i<=nc;++i)
    {
        theta[i] = theta[i-1] + (0.01+alea<double>());
    }
    
    {
        const double fac = (numeric<double>::two_pi * ( 0.9 + 0.1*alea<double>())) / theta[nc];
        for(size_t i=2;i<=nc;++i)
        {
            theta[i] *= fac;
        }
    }
    
    //--------------------------------------------------------------------------
    // construct points
    //--------------------------------------------------------------------------
    const double Rx = 2 * (1+alea<double>());
    const double Ry = (1+alea<double>());
    for( size_t i=1; i <=nc; ++i )
    {
        vtx v( Rx * Cos(theta[i]), Ry*Sin(theta[i]));
        points.push_back(v);
    }
    
    for(size_t i=2; i <= nc; ++i )
    {
        t[i] = t[i-1] + (points[i] - points[i-1]).norm();
    }
    
    
    {
        ios::ocstream fp("path.dat",false);
        for(size_t i=1; i <= nc; ++i )
        {
            fp("%g %g %g\n", t[i], points[i].x, points[i].y);
        }
    }
    
    spline2D<double> S1(t);
    S1.load(spline_natural, points.begin() );
    const size_t np = 200;
    {
        ios::ocstream fp("spath.dat",false);
        const double L = t[nc];
        for( size_t i=0; i<=np;++i)
        {
            const double u = (i*L)/np;
            const vtx    v = S1(u);
            fp("%g %g %g\n", u, v.x, v.y);
        }
    }
    
    //--------------------------------------------------------------------------
    // close path -> poly
    //--------------------------------------------------------------------------
    t.push_back( t[nc] + (points[nc]-points[1]).norm() );
    points.push_back( points[1] );
    {
        ios::ocstream fp("poly.dat",false);
        for(size_t i=1; i <= t.size(); ++i )
        {
            fp("%g %g %g\n", t[i], points[i].x, points[i].y);
        }
    }
    
    
    spline2D<double> S2(t);
    S2.load(spline_periodic,points.begin());
    {
        ios::ocstream fp("spoly.dat",false);
        const double L = t.back();
        for( size_t i=0; i<=np;++i)
        {
            const double u = (i*L)/np;
            const vtx    v = S2(u);
            fp("%g %g %g\n", u, v.x, v.y);
        }
    }
    
}
YOCTO_UNIT_TEST_DONE()
