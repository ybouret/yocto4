#include "yocto/ios/ocstream.hpp"
#include <iostream>
#include <cstdio>
#include "yocto/code/utils.hpp"
#include "yocto/sys/wtime.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/types.hpp"
#include "yocto/code/bswap.hpp"
#include <typeinfo>
#include "yocto/string/conv.hpp"
#include "yocto/exception.hpp"

using namespace yocto;
using namespace math;


#define ITER_MAX (16*1024)

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

int main(int argc, char *argv[] )
{
    
    try
    {
        if( argc <= 1 )
            throw exception("Need pmax");
        
        const size_t pmax = strconv::to<size_t>(argv[1],"pmax");
        if(argc>2&&strcmp(argv[2],"perf")==0)
        {
            ios::ocstream fp("bitrev.dat",false);
            for( size_t p=0; p <= pmax; ++p )
            {
                const size_t size = 1 << p;
                std::cerr << "size=" << size << std::endl;
                const double speed4 = perf<float>(p);
                const double speed8 = perf<double>(p);
                fp("%u %g %g\n", unsigned(size),speed4,speed8);
                std::cerr << std::endl;
            }
        }
        
        
        vector<size_t> indx(1024*1024,as_capacity);
        for( size_t p=0; p <= pmax; ++p )
        {
            indx.free();
            size_t       imax=0;
            const size_t size = 1 << p;
            std::cerr << "size=" << size << " | p=" << p << std::endl;
            size_t nops = 0;
            const size_t n    = size << 1;
            {
                size_t j=1;
                for (size_t i=1; i<n; i+=2) {
                    if (j > i)
					{
                        ++nops;
                        //std::cerr << '(' << i << ',' << j << ')' << ' ';
                        indx.push_back(i);
                        indx.push_back(j);
                        if(i>imax) imax=i;
                        if(j>imax) imax=j;
                    }
                    size_t m = size; // m=  n / 2;
                    while (m >= 2 && j > m)
					{
                        j -= m;
                        m >>= 1;
                    }
                    j += m;
                }
            }
            assert( 2*nops == indx.size());
            const size_t nw = nops * 2;
            std::cerr << "\tnops = " << nops << " =>" <<  nw <<   " words"<< std::endl;
            const size_t nb = nw * sizeof(uint32_t);
            std::cerr << "\timax = " << imax << std::endl;
            std::cerr << "\tnb   = " << nb << " / " << double(nb)/(1024.0) << " kBytes" << std::endl;
            //std::cerr << indx << std::endl;
            std::cerr << std::endl;
        }
        
        
        
        return 0;
    }
    catch( const exception &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.when() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Error" << std::endl;
    }
    return 1;
}
