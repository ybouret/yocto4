#include "yocto/ios/ocstream.hpp"
#include <iostream>
#include <cstdio>
#include "yocto/code/utils.hpp"
#include "yocto/wtime.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/types.hpp"
#include "yocto/code/bswap.hpp"
#include <typeinfo>

using namespace yocto;
using namespace math;


#define ITER_MAX 8192

template <typename T>
static inline
void perf( size_t p )
{
    wtime chrono;
    chrono.start();
    const size_t size = 1 << p;
    const size_t n    = size << 1;
    double       ell  = 0;
    vector<T> arr(n, numeric<T>::zero );
    for( size_t iter=0; iter < ITER_MAX; ++iter)
    {
        const double stamp = chrono.query();
        size_t j=1;
        for (size_t i=1; i<n; i+=2)
        {
            if (j > i)
            {
#if 0
                cswap(arr[i],arr[j]);
                cswap(arr[i+1],arr[j+1]);
                
#else
                core::bswap<2*sizeof(T)>( &arr[i], &arr[j] );
#endif
            }
            size_t m = size; // m=  n / 2;
            while (m >= 2 && j > m)
            {
                j -= m;
                m >>= 1;
            }
            j += m;
        }
        ell += (chrono.query() - stamp);
    }
    const double fac = double(ITER_MAX) * double(size) * 1e-6;
    std::cerr.flush();
    fprintf( stderr, "type=%-8s | speed%-5u = %10.5f M/s\n", typeid(T).name(), unsigned(size), fac/ell);
    fflush(stderr);
}

int main(int argc, char *argv[] )
{
    
    try
    {
        ios::ocstream decl("bitrev-tab.hpp",false);
        ios::ocstream impl("bitrev-tab.cpp",false);
        
        decl << "#ifndef YOCTO_BITREV_TAB_INCLUDED\n";
        decl << "#define YOCTO_BITREV_TAB_INCLUDED 1\n";
        
        impl << "#include \"./bitrev-tab.hpp\"\n";
        
        size_t sum_words  = 0;
        for( size_t p=0; p <= 16; ++p )
        {
            const size_t size = 1 << p;
            std::cerr << "size=" << size << std::endl;
            size_t nops = 0;
            const size_t n    = size << 1;
            {
                size_t j=1;
                for (size_t i=1; i<n; i+=2) {
                    if (j > i)
					{
                        ++nops;
                        //std::cerr << '(' << i << ',' << j << ')' << ' ';
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
            std::cerr << "\tnops = " << nops << " =>" <<  nops*2 <<   " words"<< std::endl;
            sum_words += 2*nops;
            std::cerr << std::endl;
        }
        decl << "#endif\n";
        std::cerr << "#words=" << sum_words << std::endl;
        
        if(argc>1 && 0 == strcmp(argv[0], "perf") )
        {
            for( size_t p=0; p <= 13; ++p )
            {
                const size_t size = 1 << p;
                std::cerr << "size=" << size << std::endl;
                perf<float>(p);
                perf<double>(p);
                std::cerr << std::endl;
                
            }
        }
        return 0;
    }
    catch(...)
    {
        std::cerr << "Error" << std::endl;
    }
    return 1;
}
