#include "yocto/utest/run.hpp"
#include "yocto/math/fft/fftn.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace math;


template <typename T>
static inline void perform_fftn()
{
    for(size_t nr=1;nr<=1024;nr*=2)
    {

        for(size_t nc=1;nc<=1024;nc*=2)
        {
            matrix< complex<T> > M(nr,nc);
            for(size_t i=1;i<=nr;++i)
            {
                for(size_t j=1;j<=nc;++j)
                {
                    M[i][j].re = alea<T>();
                }
            }
            matrix< complex<T> > M0 = M;
            FFT(M);
            iFFT(M);
            T delta = 0;
            for(size_t i=1;i<=nr;++i)
            {
                for(size_t j=1;j<=nc;++j)
                {
                    const T d = Fabs(M[i][j]-M0[i][j]);
                    delta += d*d;
                }
            }
            delta = Sqrt(delta/(nr*nc));
            std::cerr << " " << delta;

        }
        std::cerr << std::endl;
        //break;
    }
}

#include <cstring>
#include "yocto/sys/timings.hpp"
#include "yocto/sequence/vector.hpp"

YOCTO_UNIT_TEST_IMPL(fftn)
{
    const bool perf = (argc>1) && 0==strcmp(argv[1],"perf");

    if(!perf)
    {
        std::cerr << "-- float" << std::endl;
        perform_fftn<float>();

        std::cerr << "-- double" << std::endl;
        perform_fftn<double>();
    }
    else
    {
        vector<double> speed;
        timings tmx;
        size_t count=0;
        std::cerr.flush();
        for(size_t nr=4;nr<=256;nr*=2)
        {
            for(size_t nc=4;nc<=256;nc*=2)
            {
                ++count;
                matrix< complex<double> > M(nr,nc);
                for(size_t i=1;i<=nr;++i)
                {
                    for(size_t j=1;j<=nc;++j)
                    {
                        M[i][j].re = alea<double>();
                    }
                }
                YOCTO_TIMINGS(tmx,0.5, FFT(M); iFFT(M) );
                const double spd = tmx.speed/1000;
                speed.push_back(spd);
                fprintf(stderr, "%4d x %4d : %12.5lf kops\n", unsigned(nr), unsigned(nc), spd);
                fflush(stderr);
            }
        }
        std::cerr << "count=" << count << std::endl;
    }
}
YOCTO_UNIT_TEST_DONE()
