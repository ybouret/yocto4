#include "yocto/utest/run.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/math/types.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/string/conv.hpp"

using namespace yocto;
using namespace math;


#include "yocto/math/sig/smoother.hpp"

YOCTO_UNIT_TEST_IMPL(extend)
{
    const size_t    n=150;
    vector<double>  x(n,0.0);
    vector<double>  y(n,0.0);
    vector<double>  z(n,0.0);
    
    double dt = 0.2;
    size_t degree = 2;
    
    double NOISE = 0.4;
    
    if(argc>1)
    {
        dt = strconv::to<double>(argv[1],"dt");
    }
    
    if(argc>2)
    {
        degree = strconv::to<size_t>(argv[2],"degree");
    }
    
    if(argc>3)
    {
        NOISE = strconv::to<double>(argv[3],"NOISE");
    }
    
    for( size_t i=2; i <= n; ++i )
    {
        x[i] = x[i-1] + 0.5 + alea<double>();
    }
    
    const double fac = (numeric<double>::two_pi / x[n]);
    
    for( size_t i=1; i <= n; ++i )
    {
        x[i] *= fac;
        y[i] = 0.2+sin(x[i]) + sin(3*x[i]);
        z[i] = y[i] + NOISE * ( 0.5 - alea<double>() );
    }
    y[n] = y[1];
    z[1] = y[1];
    z[n] = y[n];
    
    extender<double> xtd1(extend_constant,extend_constant);
    extender<double> xtd2(extend_cyclic,extend_cyclic);
    extender<double> xtd3(extend_odd,extend_odd);
    extender<double> xtd4(extend_even,extend_even);
    
    std::cerr << "x1[0]=" << xtd1.get_x(0,x,n) << std::endl;
    std::cerr << "x2[0]=" << xtd2.get_x(0,x,n) << std::endl;
    std::cerr << "x3[0]=" << xtd3.get_x(0,x,n) << std::endl;
    std::cerr << "x4[0]=" << xtd4.get_x(0,x,n) << std::endl;
    
    {
        ios::ocstream fp("xdata.dat", false);
        for( size_t i=1; i<=n; ++i )
        {
            fp("%g %g %g\n", x[i], y[i], z[i]);
        }
    }
    
    
    {
        ios::ocstream fp("xivtx.dat",false);
        for( ptrdiff_t i= -ptrdiff_t(2*n); i <= ptrdiff_t(n+2*n); ++i)
        {
            if(true)
            {
                fp("%g %g", xtd1.get_x(i,x, n), xtd1.get_y(i,y,n));
            }
            
            if(true)
            {
                fp(" %g %g", xtd2.get_x(i,x, n), xtd2.get_y(i,y,n));
            }
            
            if(true)
            {
                fp(" %g %g", xtd3.get_x(i,x, n), xtd3.get_y(i,y,n));
            }
            
            if(true)
            {
                fp(" %g %g", xtd4.get_x(i,x, n), xtd4.get_y(i,y,n));
            }
            fp("\n");
        }
    }
    
    smoother<double> sm;
    
    sm.upper_range = dt/2;
    sm.lower_range = dt/2;
    sm.degree      = degree;
    
    
    vector<double> z2(n,0.0);
    vector<double> w2(n,0.0);
    sm.run(z2,x,z,xtd2,&w2);
    {
        ios::ocstream fp("xz2.dat", false);
        for(size_t i=1; i<=n; ++i )
        {
            fp("%g %g %g\n", x[i], z2[i], w2[i]);
        }
    }
    
    vector<double> z1(n,0.0);
    vector<double> w1(n,0.0);
    sm.run(z1,x,z,xtd1,&w1);
    {
        ios::ocstream fp("xz1.dat", false);
        for(size_t i=1; i<=n; ++i )
        {
            fp("%g %g %g\n", x[i], z1[i], w1[i]);
        }
    }
    
    vector<double> z3(n,0.0);
    vector<double> w3(n,0.0);
    sm.run(z3,x,z,xtd3,&w3);
    {
        ios::ocstream fp("xz3.dat", false);
        for(size_t i=1; i<=n; ++i )
        {
            fp("%g %g %g\n", x[i], z1[i], w1[i]);
        }
    }
    
    sm.full(z2,x,z,xtd2,w2);
    {
        ios::ocstream fp("xw2.dat", false);
        for(size_t i=1; i<=n; ++i )
        {
            fp("%g %g %g\n", x[i], z2[i], w2[i]);
        }
    }
}
YOCTO_UNIT_TEST_DONE()


YOCTO_UNIT_TEST_IMPL(extend2)
{
    const size_t    n=150;
    vector<double>  x(n,0.0);
    vector<double>  y(n,0.0);
    vector<double>  z(n,0.0);
    
    double dt     = 0.2;
    size_t degree =   2;
    
    double NOISE = 0.1;
    
    if(argc>1)
    {
        dt = strconv::to<double>(argv[1],"dt");
    }
    
    if(argc>2)
    {
        degree = strconv::to<size_t>(argv[2],"degree");
    }
    
    if(argc>3)
    {
        NOISE = strconv::to<double>(argv[3],"NOISE");
    }
    
    for( size_t i=2; i <= n; ++i )
    {
        x[i] = x[i-1] + 0.5 + alea<double>();
    }
    const double fac = n*(1.0 / x[n])/double(n+1);
    
    for( size_t i=1; i <= n; ++i )
    {
        x[i] *= fac;
        y[i] = exp(-x[i]);
        z[i] = y[i] + NOISE * ( 0.5 - alea<double>() );
    }
    
    extender<double> xtd(extend_odd);
    
    smoother<double> sm;
    sm.lower_range = dt/2;
    sm.upper_range = dt/2;
    sm.degree      = degree;
    
    vector<double> zf(n,0.0);
    vector<double> wf(n,0.0);
    sm.full(zf,x,z,xtd,wf);
    
    {
        ios::ocstream fp("xdata2.dat",false);
        for(size_t i=1;i<=n;++i)
        {
            fp("%g %g %g %g %g\n", x[i], y[i], z[i], zf[i], wf[i]);
        }
    }
    
}
YOCTO_UNIT_TEST_DONE()

