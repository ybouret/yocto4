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
#include "yocto/sort/quick.hpp"

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

static inline
void output( const array<size_t> &u, ios::ostream &fp )
{
    const size_t n = u.size();
    for(size_t i=1;i<n;++i)
    {
        fp("%u, ", unsigned(u[i]-1));
        if(0==(i%16)) fp("\n");
    }
    fp("%u\n", unsigned(u[n]-1));
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
        vector<size_t> jndx(1024*1024,as_capacity);
        
        ios::ocstream fp("bitrevtab.cxx",false);
        ios::ocstream src("bitrevcode.cxx",false);
        ios::ocstream src2("bitrevcode2.cxx",false);
        
        src("#define BRSZ 2*sizeof(real_t)\n");
        src("void __bitrev( real_t *arr, size_t size )\n");
        src("{\n");
        src("\tswitch(size)\n");
        src("\t{\n");
        src("\tcase 0:\n\tcase 1:\n\tcase 2:\n\tbreak;\n");
        
        src2("void __bitrev( real_t *arr, real_t *brr, size_t size )\n");
        src2("{\n");
        src2("\tswitch(size)\n");
        src2("\t{\n");
        src2("\tcase 0:\n\tcase 1:\n\tcase 2:\n\tbreak;\n");
        
        
        for( size_t p=0; p <= pmax; ++p )
        {
            indx.free();
            jndx.free();
            size_t       imax = 0;
            const size_t size = 1 << p;
            std::cerr << "size=" << size << " | p=" << p << std::endl;
            size_t nops = 0;
            const size_t n    = size << 1;
            {
                size_t j=1;
                for (size_t i=1; i<n; i+=2)
                {
                    if (j > i)
					{
                        ++nops;
                        //std::cerr << '(' << i << ',' << j << ')' << ' ';
                        indx.push_back(i);
                        jndx.push_back(j);
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
            std::cerr << std::endl;
            assert( nops == indx.size() );
            assert( nops == jndx.size() );
            co_qsort(indx, jndx);
            const size_t nw = nops * 2;
            std::cerr << "\tnops = " << nops << " =>" <<  nw <<   " words"<< std::endl;
            const size_t nb = nw * sizeof(uint32_t);
            std::cerr << "\timax = " << imax << std::endl;
            std::cerr << "\tnb   = " << nb << " / " << double(nb)/(1024.0) << " kBytes" << std::endl;
            //std::cerr << indx << std::endl;
            std::cerr << std::endl;
            if(nops)
            {
                fp("// BitRev for size=%u\n", unsigned(size) );
                fp("static const size_t   num%u = %u;\n", unsigned(size), unsigned(nops));
                fp("static const word_t idx%u[%u] = {\n", unsigned(size), unsigned(nops) );
                output( indx, fp );
                fp("};\n");
                fp("static const word_t jdx%u[%u] = {\n", unsigned(size), unsigned(nops) );
                output( jndx, fp );
                fp("};\n");
                fp("\n");
                
                src("\tcase %u:\n", unsigned(size));
                src2("\tcase %u:\n", unsigned(size));
                for(size_t i=1;i<=nops;++i)
                {
                    src("\t\tcore::bswap<BRSZ>(&arr[%4u], &arr[%4u]);\n", unsigned(indx[i]-1), unsigned(jndx[i]-1));
                    src2("\t\tcore::bswap<BRSZ>(&arr[%4u], &arr[%4u]);\n", unsigned(indx[i]-1), unsigned(jndx[i]-1));
                    src2("\t\tcore::bswap<BRSZ>(&brr[%4u], &brr[%4u]);\n", unsigned(indx[i]-1), unsigned(jndx[i]-1));
                    
                }
                src("\treturn;\n");
                src2("\treturn;\n");
            }
        }
        src("\tdefault:\n");
        src("\t\t{\n");
        src(
            "\t\t\t--arr;\n"
            "\t\t\tconst size_t n = size << 1;\n"
            "\t\t\tsize_t j=1;\n"
            "\t\t\tfor (size_t i=1; i<n; i+=2)\n"
            "\t\t\t{\n"
            "\t\t\t\tif (j > i)\n"
            "\t\t\t\t{\n"
            "\t\t\t\t\tcore::bswap<BRSZ>( &arr[i], &arr[j] );\n"
            "\t\t\t\t}\n"
            "\t\t\t\tsize_t m = size;\n"
            "\t\t\t\twhile (m >= 2 && j > m)\n"
            "\t\t\t\t{\n"
            "\t\t\t\t\tj -=  m;\n"
            "\t\t\t\t\tm >>= 1;\n"
            "\t\t\t\t}\n"
            "\t\t\t\tj += m;\n"
            "\t\t\t}\n"
            );
        
        src("\t\t}\n");
        src("\t}\n");
        src("}\n");
        
        src2("\tdefault:\n");
        src2("\t\t{\n");
        src2(
             "\t\t\t--arr;\n"
             "\t\t\t--brr;\n"
             "\t\t\tconst size_t n = size << 1;\n"
             "\t\t\tsize_t j=1;\n"
             "\t\t\tfor (size_t i=1; i<n; i+=2)\n"
             "\t\t\t{\n"
             "\t\t\t\tif (j > i)\n"
             "\t\t\t\t{\n"
             "\t\t\t\t\tcore::bswap<BRSZ>( &arr[i], &arr[j] );\n"
             "\t\t\t\t\tcore::bswap<BRSZ>( &brr[i], &brr[j] );\n"
             "\t\t\t\t}\n"
             "\t\t\t\tsize_t m = size;\n"
             "\t\t\t\twhile (m >= 2 && j > m)\n"
             "\t\t\t\t{\n"
             "\t\t\t\t\tj -=  m;\n"
             "\t\t\t\t\tm >>= 1;\n"
             "\t\t\t\t}\n"
             "\t\t\t\tj += m;\n"
             "\t\t\t}\n"
             );
        
        src2("\t\t}\n");
        src2("\t}\n");
        src2("}\n");
        
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
