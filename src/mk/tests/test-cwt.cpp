#include "yocto/math/wavelet/cwt.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace math;


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

#include "yocto/string/conv.hpp"
#include "yocto/sys/wtime.hpp"

YOCTO_UNIT_TEST_IMPL(cwt)
{
    size_t N = 200;
    if(argc>1)
    {
        const string num = argv[1];
        N = strconv::to<size_t>(num,"N");
    }
    vector<double> x(N,as_capacity);
    vector<double> y(N,as_capacity);

    const double width = 10;
    alea_init();

    x.push_back(0);
    for(size_t i=1;i<N;++i)
    {
        x.push_back( x.back() + 0.1 + alea<double>() );
    }
    {
        assert(x.size()==N);
        const double x_end = x[N];
        for(size_t i=1;i<=N;++i)
        {
            x[i] *= width;
            x[i] /= x_end;
        }
    }

    for(size_t i=1;i<=N;++i)
    {
        const double xx = x[i];
        //const double yy = //0.4 + sin(xx);// + 1.2 * sin(2.1*xx);
        const double dx = (xx-4);
        const double yy = 0.3 + 0.1 *exp(-1.1*dx*dx) + 0.02 * ( 0.5 - alea<double>());
        y.push_back(yy);
    }


    //co_qsort(x,y);
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
    double (*Kernel)(double) = Gaussian;
    if(argc>2)
    {
        const string id = argv[2];
        if(id=="MexicanHat")
        {
            Kernel = wavelet<double>::Ricker;
        }

        if(id=="Shannon")
        {
            Kernel = Shannon;
        }
    }

    numeric<double>::function   Psi(Kernel);
    wtime chrono;
    chrono.start();


    std::cerr << "Raw Transform..." << std::endl;
    double t_ini = chrono.query();
    wavelet<double>::cwt_raw(x, y, Psi,shifts,scales, W);
    const double t_raw = chrono.query() - t_ini;
    std::cerr << "t_raw=" << t_raw << std::endl;
    {
        ios::ocstream fp("cwt_raw.dat",false);
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

    std::cerr << "Optimized Transform..." << std::endl;
    t_ini = chrono.query();
    wavelet<double>::cwt(x, y, Psi,shifts,scales, W);
    const double t_opt = chrono.query() - t_ini;
    std::cerr << "t_opt=" << t_opt << std::endl;
    {
        ios::ocstream fp("cwt.dat",false);
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


    std::cerr << "Fullly Optimized Transform..." << std::endl;
    t_ini = chrono.query();
    wavelet<double>::cwt_full(x, y, Psi,shifts,scales, W);
    const double t_ful = chrono.query() - t_ini;
    std::cerr << "t_ful=" << t_ful << std::endl;
    {
        ios::ocstream fp("cwt_ful.dat",false);
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
