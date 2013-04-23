#include "yocto/ios/ocstream.hpp"
#include <iostream>
#include <cstdio>
#include "yocto/code/utils.hpp"
#include "yocto/wtime.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/types.hpp"
#include "yocto/code/bswap.hpp"
#include <typeinfo>
#include "yocto/string/conv.hpp"
#include "yocto/exception.hpp"

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
        if( argc <= 1 )
            throw exception("Need pmax");
        
        const size_t pmax = strconv::to<size_t>(argv[1],"pmax");
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
