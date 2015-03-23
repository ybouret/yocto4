#include "yocto/program.hpp"
#include "xbitrev.hpp"
#include "yocto/string/conv.hpp"
#include "yocto/math/types.hpp"
#include "yocto/sys/wtime.hpp"
#include "yocto/sequence/vector.hpp"
#include <cstdio>

using namespace yocto;
using namespace math;

#define ITER_MAX (64*1024)

#if 0
template <typename T>
static inline
double perf( const size_t p )
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

    const double ell   = chrono(t64);
    const double fac   = double(ITER_MAX) * double(size) * 1e-6;
    const double speed = fac/ell;
    std::cerr.flush();
    fprintf( stderr, "type=%-8s | speed%-5u = %10.5f M/s\n", typeid(T).name(), unsigned(size), speed);
    fflush(stderr);
    return speed;
}
#endif


template <typename T>
static inline
void perf_cplx(const size_t p)
{
    //std::cerr << std::endl;
    const size_t size = 1 << p;
    std::cerr << "size=" << size << std::endl;

    typedef complex<T> cplx;
    vector< cplx > u(size,as_capacity);
    for(size_t i=1;i<=size;++i)
    {
        const cplx c( i,i );
        u.push_back(c);
    }

    vector< cplx > v(u); assert(v.size()==size);
    //vector< cplx > z(u); assert(z.size()==size);


    // testing
    xbitrev::run_safe(&v[1].re-1, size);
    xbitrev::run_safe(&v[1].re-1, size);

    for(size_t i=1;i<=size;++i)
    {
        if(v[i]!=u[i]) throw exception("NON INVOLUTIVE SAFE BITREV");
    }

    // perf
    wtime chrono;
    chrono.start();

    {
        uint64_t t64 = 0;
        for( size_t iter=0; iter < ITER_MAX; ++iter)
        {
            const uint64_t stamp = wtime::ticks();
            xbitrev::run_safe(&v[1].re-1, size);
            t64 += wtime::ticks() - stamp;
        }
        const double ell   = chrono(t64);
        const double fac   = double(ITER_MAX) * double(size) * 1e-6;
        const double speed = fac/ell;
        std::cerr.flush();
        fprintf( stderr, "RAW type=%-8s | speed%-5u = %10.5f M/s\n", typeid(T).name(), unsigned(size), speed);
        fflush(stderr);
    }

    // testing
    {
        vector< cplx > w(u); assert(w.size()==size);
        xbitrev::run(&w[1].re-1, size);
        xbitrev::run(&w[1].re-1, size);

        for(size_t i=1;i<=size;++i)
        {
            if(w[i]!=u[i]) throw exception("NON INVOLUTIVE BITREV,size=%u",unsigned(size));
        }
    }


    {
        uint64_t t64 = 0;
        for( size_t iter=0; iter < ITER_MAX; ++iter)
        {
            const uint64_t stamp = wtime::ticks();
            xbitrev::run(&v[1].re-1, size);
            t64 += wtime::ticks() - stamp;
        }
        const double ell   = chrono(t64);
        const double fac   = double(ITER_MAX) * double(size) * 1e-6;
        const double speed = fac/ell;
        std::cerr.flush();
        fprintf( stderr, "XBR type=%-8s | speed%-5u = %10.5f M/s\n", typeid(T).name(), unsigned(size), speed);
        fflush(stderr);
    }




}


YOCTO_PROGRAM_START()
{
    size_t pmax = 5;
    if(argc>1)
    {
        pmax = strconv::to<size_t>(argv[1],"pmax");
    }

    for(size_t p=0;p<=pmax;++p)
    {
        perf_cplx<float>(p);
    }
    std::cerr << std::endl;

    for(size_t p=0;p<=pmax;++p)
    {
        perf_cplx<double>(p);
    }
    std::cerr << std::endl;
    
    
}
YOCTO_PROGRAM_END()
