#include "yocto/program.hpp"
#include "yocto/sys/wtime.hpp"
#include "yocto/math/types.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;
using namespace math;

#define ITER_MAX (64*1024)


template <typename T>
static inline
double perf( size_t p )
{
    wtime chrono;
    chrono.start();
    const size_t size = 1 << p;
    const size_t n    = size << 1;
    vector<T> arr(n, numeric<T>::zero );
    uint64_t t64 = 0;
    for( size_t iter=0; iter < ITER_MAX; ++iter)
    {
        const uint64_t stamp = wtime::ticks();
        size_t j=1;
        for (size_t i=1; i<n; i+=2)
        {
            if (j > i)
            {
                core::bswap<2*sizeof(T)>( &arr[i], &arr[j] );
            }
            size_t m = size; // m=  n / 2;
            while (m >= 2 && j > m)
            {
                j -=  m;
                m >>= 1;
            }
            j += m;
        }
        t64 += wtime::ticks() - stamp;
    }

    const double ell = chrono(t64);
    const double fac = double(ITER_MAX) * double(size) * 1e-6;
    const double speed = fac/ell;
    std::cerr.flush();
    fprintf( stderr, "type=%-8s | speed%-5u = %10.5f M/s\n", typeid(T).name(), unsigned(size), speed);
    fflush(stderr);
    return speed;
}


YOCTO_PROGRAM_START()
{
    const size_t pmax = 12;

    for(size_t p=0;p<=pmax;++p)
    {
        (void) perf<float>(p);
    }
    std::cerr << std::endl;
    for(size_t p=0;p<=pmax;++p)
    {
        (void) perf<double>(p);
    }
}
YOCTO_PROGRAM_END()
