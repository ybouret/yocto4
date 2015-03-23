#include "yocto/program.hpp"
#include "yocto/sys/wtime.hpp"
#include "yocto/math/types.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/string/conv.hpp"

using namespace yocto;
using namespace math;

#define ITER_MAX (64*1024)


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

    const double ell = chrono(t64);
    const double fac = double(ITER_MAX) * double(size) * 1e-6;
    const double speed = fac/ell;
    std::cerr.flush();
    fprintf( stderr, "type=%-8s | speed%-5u = %10.5f M/s\n", typeid(T).name(), unsigned(size), speed);
    fflush(stderr);
    return speed;
}

#include "yocto/sort/quick.hpp"

static inline
void compute_swaps( const size_t p, sequence<size_t> &swaps )
{
    const size_t size = 1 << p;
    const size_t n    = size << 1;

    swaps.free();

    vector<size_t> indx(n,as_capacity);
    vector<size_t> jndx(n,as_capacity);

    size_t nops = 0;

    register size_t j=1;
    for (register size_t i=1; i<n; i+=2)
    {
        if (j > i)
        {
            //core::bswap<2*sizeof(T)>( &arr[i], &arr[j] );
            //std::cerr << i  << ", " << j << std::endl;
            indx.push_back(i);
            jndx.push_back(j);
            ++nops;
        }
        size_t m = size; // m=  n / 2;
        while (m >= 2 && j > m)
        {
            j -=  m;
            m >>= 1;
        }
        j += m;
    }
    assert(indx.size()==nops);
    assert(jndx.size()==nops);
    co_qsort(indx, jndx);
    std::cerr << " nops" << size << " = " << nops << std::endl;
    std::cerr << "words" << size << " = " << nops * 2 << std::endl;
    //std::cerr << indx << std::endl;
    //std::cerr << jndx << std::endl;
    //return nops;
    for(size_t i=1;i<=nops;++i)
    {
        swaps.push_back(indx[i]);
        swaps.push_back(jndx[i]);
    }
}

#include <cstring>
#include "yocto/ios/ocstream.hpp"

YOCTO_PROGRAM_START()
{
    size_t pmax = 5;
    if(argc>1)
    {
        pmax = strconv::to<size_t>(argv[1],"pmax");
    }

    if(argc>2&& !strcmp(argv[2],"perf"))
    {
        for(size_t p=0;p<=pmax;++p)
        {
            (void) perf<float>(p);
        }
        std::cerr << std::endl;
        for(size_t p=0;p<=pmax;++p)
        {
            (void) perf<double>(p);
        }
        std::cerr << std::endl;
    }

    vector<size_t> swaps(65536,as_capacity);
    ios::ocstream hdr("xbitrev.hpp",false);
    ios::ocstream src("xbitrev.cpp",false);

    hdr << "#ifndef YOCTO_XBITREV_INCLUDED\n";
    hdr << "#define YOCTO_XBITREV_INCLUDED 1\n\n";
    hdr << "#include \"yocto/code/bswap.hpp\"\n";
    hdr << "namespace yocto { namespace math {\n\n";

    src << "#include \"./xbitrev.hpp\"\n";
    src << "namespace yocto { namespace math {\n\n";

    hdr << "\tstruct xbitrev {\n";

    for(size_t p=0;p<=pmax;++p)
    {
        const size_t sz = 1 << p;
        compute_swaps(p,swaps);
        const size_t nw = swaps.size();
        //const size_t ns = nw/2;
        hdr("\t\tstatic const size_t indx%04u[%4u];\n",unsigned(sz),unsigned(nw));
        src("\tconst size_t xbitrev::indx%04u[%4u]= {",unsigned(sz),unsigned(nw) );
        for(size_t i=1;i<=nw;++i)
        {
            if( 0 == ((i-1)&15) )
            {
                src << "\n\t";
            }
            src("%4u", unsigned(swaps[i]));
            if(i<nw)
            {
                src << ',';
            }
        }

        src("\t};\n\n");

    }

    // generic code
    hdr << "\n\t\ttemplate <typename T> inline void run_safe( T arr[], const size_t size) throw() {\n";
    hdr << "\t\t\t const    size_t n = size << 1;\n";
    hdr << "\t\t\t register size_t j = 1;\n";
    hdr << "\t\t\t for(register size_t i=1; i<n; i+=2)\n";
    hdr << "\t\t\t {\n";
    hdr << "\t\t\t     if(j>i)\n";
    hdr << "\t\t\t     {\n";
    hdr << "\t\t\t         core::bswap<2*sizeof(T)>( &arr[i], &arr[j] );\n";
    hdr << "\t\t\t     }\n";
    hdr << "\t\t\t    register size_t m = size;\n";
    hdr << "\t\t\t    while( (m>=2) && (j>m) )\n";
    hdr << "\t\t\t    {\n";
    hdr << "\t\t\t       j -=  m;\n";
    hdr << "\t\t\t       m >>= 1;\n";
    hdr << "\t\t\t    }\n";
    hdr << "\t\t\t    j += m;\n";
    hdr << "\t\t\t }\n";

    hdr << "\t\t}\n\n";

    hdr << "\t};\n";

    hdr << "\n} }\n";
    src << "\n} }\n";
    hdr << "#endif\n";
    
    
}
YOCTO_PROGRAM_END()
