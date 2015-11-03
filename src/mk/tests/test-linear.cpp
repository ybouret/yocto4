#include "yocto/utest/run.hpp"
#include "yocto/math/dat/linear.hpp"
#include "yocto/code/rand.hpp"

#include "yocto/sequence/vector.hpp"
#include "yocto/math/types.hpp"

#include "yocto/ios/ocstream.hpp"

using namespace yocto;

static inline double F(double x)
{
    return cos(x);
}

YOCTO_UNIT_TEST_IMPL(linear)
{

    vector<double> x;
    vector<double> y;
    
    x.push_back(0);
    y.push_back( F(x.back()) );
    for( size_t i=200 + alea_leq(100); i>0; --i )
    {
        x.push_back( x.back() + 0.1 * alea<double>() );
        y.push_back( F(x.back()) );
    }
    
    {
        ios::ocstream fp( "raw.dat", false );
        for( size_t i=1; i <= x.size(); ++i )
            fp("%g %g\n", x[i], y[i] );
    }
    
    const size_t n  = x.size();
    const double x1 = x[1];
    const double xn = x[n];
    const double delta_x = xn - x1;
    
    {
        ios::ocstream fp( "lin.dat", false );
        const size_t N = 60;
        for( size_t i=0; i <= N; ++i )
        {
            const double xi = x1 + (i*delta_x)/N;
            fp("%g %g\n", xi, math::linear<double>(xi,x,y) );
        }
    }
    

}
YOCTO_UNIT_TEST_DONE()
