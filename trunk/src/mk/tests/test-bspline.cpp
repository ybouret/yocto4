#include "yocto/math/alg/b-spline.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/rand32.hpp"
#include "yocto/sequence/vector.hpp"

#include "yocto/ios/ocstream.hpp"
#include "yocto/math/types.hpp"

using namespace yocto;
using namespace math;

YOCTO_UNIT_TEST_IMPL(bspline)
{
    
    uniform_generator<double> ran;
    
    const size_t N = 7;
    vector< double >      U;
    vector< v2d<double> > V2;
    vector< v3d<double> > V3;
    
    
    {
        double       xx = 0;
        for(size_t i=1;i<=N;++i)
        {
            U.push_back(xx); xx += 0.1 + ran();
        }
        xx = U.back();
        for(size_t i=1;i<=N;++i)
        {
            U[i] /= xx;
        }
        U[N] = 1;
        
        
        
    }
    
    
    {
        const double fac = 6.28/(N+1);
        
        for(size_t i=1;i<=N;++i)
        {
            const double xx = (1 + 0.1*(ran()-0.5) ) * cos( i * fac );
            const double yy = (1 + 0.1*(ran()-0.5) ) * sin( i * fac );
            const double zz = U[i];
            const v2d<double> v2(xx,yy);
            const v3d<double> v3(xx,yy,zz);
            V2.push_back(v2);
            V3.push_back(v3);
        }
    }
    
    {
        ios::ocstream fp("v2.dat",false);
        for(size_t i=1;i<=N;++i)
        {
            fp("%g %g %g\n", U[i], V2[i].x, V2[i].y );
        }
    }
    
    {
        ios::ocstream fp("v3.dat",false);
        for(size_t i=1;i<=N;++i)
        {
            fp("%g %g %g %g\n", U[i], V3[i].x, V3[i].y, V3[i].z );
        }
    }
    
    {
        ios::ocstream fp2("v2s.dat",false);
        const size_t NN = N * 10;
        for(size_t i=0;i<=NN;++i)
        {
            const double      x  = i/double(NN);//(i<N) ? i/double(NN) : 1.0;
            const v2d<double> v2 = BSpline(x, U, V2);
            fp2("%g %g %g\n", x, v2.x, v2.y );

        }
    }
    
    
    
}
YOCTO_UNIT_TEST_DONE()
