#include "yocto/utest/run.hpp"
#include "yocto/math/sig/data-smoother.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/math/types.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/string/conv.hpp"

using namespace yocto;
using namespace math;

YOCTO_UNIT_TEST_IMPL(smooth)
{
    
    size_t nf = 5;
    size_t d  = 2;
    
    if( argc > 1 )
    {
        nf = strconv::to_size(argv[1],"nf");
    }
    
    if( argc > 2 )
    {
        d = strconv::to_size(argv[2],"d");
    }
    
    const size_t    n=200;
    vector<double>  x(n,0.0);
    vector<double>  y(n,0.0);
    vector<double>  z(n,0.0);
    
    for( size_t i=1; i <= n; ++i )
    {
        x[i] = numeric<double>::two_pi * (i-1) / double(n);
        y[i] = sin(x[i]) + sin(3*x[i]);
        z[i] = y[i] + 0.5 * ( 0.5 - alea<double>() );
    }
    z[1] = y[1];
    z[n] = y[n];
    
    {
        ios::ocstream fp("rdata.dat", false);
        for( size_t i=1; i<=n; ++i )
        {
            fp("%g %g %g\n", x[i], y[i], z[i]);
        }
    }
    
    
    extender<double> Ex(z,extension_cyclic,extension_cyclic);
    smoother<double> Sm_u(nf,nf,smoother<double>::uniform,d);
    smoother<double> Sm_g(nf,nf,smoother<double>::gaussian,d);
    vector<double>   s_u(n,0.0);
    vector<double>   s_g(n,0.0);
    Sm_u.run(s_u,Ex,0);
    Sm_g.run(s_g,Ex,0);
    
    {
        ios::ocstream fp("smooth.dat", false);
        for( size_t i=1; i<=n; ++i )
        {
            fp("%g %g %g\n", x[i], s_u[i], s_g[i]);
        }
    }
    
    
    
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/math/sig/extend.hpp"

YOCTO_UNIT_TEST_IMPL(extend)
{
    const size_t    n=200;
    vector<double>  x(n,0.0);
    vector<double>  y(n,0.0);
    vector<double>  z(n,0.0);
    
    for( size_t i=1; i <= n; ++i )
    {
        x[i] = numeric<double>::two_pi * (i-1) / double(n);
        y[i] = sin(x[i]) + sin(3*x[i]);
        z[i] = y[i] + 0.5 * ( 0.5 - alea<double>() );
    }
    z[1] = y[1];
    z[n] = y[n];
    
    {
        ios::ocstream fp("xdata.dat", false);
        for( size_t i=1; i<=n; ++i )
        {
            fp("%g %g %g\n", x[i], y[i], z[i]);
        }
    }
    
    extend<double> xtd1(extend_constant,extend_constant);
    extend<double> xtd2(extend_cyclic,extend_cyclic);
    
    {
        ios::ocstream fp("xtend.dat",false);
        for( double xx = x[1]-1; xx <= x[n]+1; xx += 0.01 )
        {
            fp("%g %g %g\n", xx, xtd1(xx,x,z), xtd2(xx,x,z));
        }
    }
    
    
}
YOCTO_UNIT_TEST_DONE()

