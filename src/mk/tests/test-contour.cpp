#include "yocto/math/alg/contour2d.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/container/matrix.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace math;

static inline
void linecb(double x1,double y1,double x2,double y2, double level, void *args)
{
    assert(args);
    ios::ocstream &fp = *static_cast<ios::ocstream *>(args);
    fp("%g %g\n%g %g\n\n",x1,y1,x2,y2);
}

YOCTO_UNIT_TEST_IMPL(contour2d)
{
    const int      nc = 3;
    vector<double> z(nc);
    z[1] = -0.1;
    z[2] = 0;
    z[3] = 0.1;
    
    {
        double  **d = 0;
        double   *x = 0;
        double   *y = 0;
        math::contour2d(d, 0, 0, 0, 0, x, y, z, NULL,NULL);
    }
    
    {
        ios::wcstream fp("contour2d.dat");
        const size_t  n = 101;
        matrix<float>  d(n,n);
        vector<double> x(n);
        vector<double> y(n);
        for(size_t i=1;i<=n;++i)
        {
            x[i] = y[i] = double(i-1)/(n-1);
        }
        
        for(size_t i=1;i<=n;++i)
        {
            for(size_t j=1;j<=n;++j)
            {
                d[i][j] = float(Square((x[i]-0.2)/0.3) + Square((y[j]-0.1)/0.1) - 1);
            }
        }
        math::contour2d(d, 1, n, 1, n, x, y, z, linecb,&fp);
        
    }
}
YOCTO_UNIT_TEST_DONE()


