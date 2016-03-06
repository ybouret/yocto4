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
                    M[i][j].re = alea<double>();
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
                    delta += Fabs(M[i][j]-M0[i][j]);
                }
            }
            delta /= (nr*nc);
            std::cerr << " " << delta;

        }
        std::cerr << std::endl;
        //break;
    }
}

YOCTO_UNIT_TEST_IMPL(fftn)
{
    std::cerr << "-- float" << std::endl;
    perform_fftn<float>();
    
    std::cerr << "-- double" << std::endl;
    perform_fftn<double>();
}
YOCTO_UNIT_TEST_DONE()
