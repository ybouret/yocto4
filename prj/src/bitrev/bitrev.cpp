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
        
#if 0
        const size_t smax = 1 << pmax;
        ios::ocstream decl("bitrev-tab.hpp",false);
        ios::ocstream impl("bitrev-tab.cpp",false);
        
        decl << "#ifndef YOCTO_BITREV_TAB_INCLUDED\n";
        decl << "#define YOCTO_BITREV_TAB_INCLUDED 1\n";
        decl << "\n";
        decl("#define YOCTO_BITREV_SIZE_MAX %u\n\n", unsigned(smax));
        
        impl << "#include \"./bitrev-tab.hpp\"\n";
        
        
        vector<size_t> indx;
        for( size_t p=0; p <= pmax; ++p )
        {
            indx.free();
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
                        indx.push_back(i);
                        indx.push_back(j);
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
            std::cerr << std::endl;
            if(nw>0)
            {
                decl( "extern const size_t n%u;\n", unsigned(size) );
                impl( "const size_t n%u = %u;\n", unsigned(size), unsigned(nops));
                impl( "const size_t indx%u[%u] = {\n", unsigned(size), unsigned(nw));
                decl( "extern const size_t indx%u[%u];\n",unsigned(size), unsigned(nw) );
                
                for( size_t i=1; i <= nw; ++i )
                {
                    impl(" %4u", unsigned(indx[i]) );
                    if( i < indx.size() )
                        impl(",");
                    if( 0 == (i%16) ) impl("\n");
                }
                if( 0 != ( indx.size() % 16 ) ) impl << "\n";
                impl( "};\n");
            }
            
        }
        decl << "#endif\n";
#endif
        
        
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
