#include "yocto/math/wavelet/cwt.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace math;

static inline
double MexicanHat( double u )
{
    const double u2 = u*u;
    return (1.0-u2)*exp(-u2/2.0);
}

static inline
double Gaussian( double u )
{
    return exp(-u*u);
}

static inline double __sinc(double x)
{
    if(fabs(x)>1e-3)
    {
        return sin(x)/x;
    }
    else
    {
        const double x2 = x*x;
        const double x4 = x2*x2;
        return (1.0-x2/6.0 + x4/120.0);
    }
}

static inline double sinc(double x)
{
    return __sinc( numeric<double>::pi * x);
}

static inline
double Shannon(double x)
{
    return 2*sinc(2*x-1)-sinc(x);
}

YOCTO_UNIT_TEST_IMPL(cwt)
{
    const size_t N = 100;
    vector<double> x(N,as_capacity);
    vector<double> y(N,as_capacity);

    const double width = 10;
    alea_init();
    for(size_t i=1;i<=N;++i)
    {
        const double xx = alea<double>() * width;
        //const double yy = //0.4 + sin(xx);// + 1.2 * sin(2.1*xx);
        const double dx = (xx-2);
        const double yy = 0.3 + 0.1 *exp(-1.5*dx*dx);
        x.push_back(xx);
        y.push_back(yy);
    }

    co_qsort(x,y);
    {
        ios::ocstream fp("wdat.dat",false);
        for(size_t i=1;i<=N;++i)
        {
            fp("%g %g\n", x[i], y[i]);
        }
    }

    vector<double> shifts(N,0);
    vector<double> scales(N,0);

    matrix<double> W;
    //numeric<double>::function Psi(  cfunctor(MexicanHat) );
    numeric<double>::function   Psi( cfunctor(Gaussian)   );
    //numeric<double>::function Psi3( cfunctor(Shannon)   );

    wavelet<double>::cwt(x, y, Psi,shifts,scales, W);

    {
        ios::ocstream fp("cw.dat",false);
        for(size_t i=1;i<=N;++i)
        {
            const double xx = shifts[i];
            for(size_t j=1;j<=N;++j)
            {
                const double yy = scales[j];
                const double ww = W[i][j];
                fp("%g %g %g\n",xx,yy,ww*ww);
            }
            fp("\n");
        }
    }

    wavelet<double>::cwt_opt(x, y, Psi,shifts,scales, W);
    {
        ios::ocstream fp("cw2.dat",false);
        for(size_t i=1;i<=N;++i)
        {
            const double xx = shifts[i];
            for(size_t j=1;j<=N;++j)
            {
                const double yy = scales[j];
                const double ww = W[i][j];
                fp("%g %g %g\n",xx,yy,ww*ww);
            }
            fp("\n");
        }
    }



}
YOCTO_UNIT_TEST_DONE()
