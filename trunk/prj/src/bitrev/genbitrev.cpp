#include "yocto/program.hpp"
#include "yocto/sys/wtime.hpp"
#include "yocto/math/types.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/string/conv.hpp"

using namespace yocto;
using namespace math;


#include "yocto/sort/quick.hpp"

static inline
void compute_swaps(const size_t    p,
                   vector<size_t> &indx,
                   vector<size_t> &jndx

                   )
{
    const size_t size = 1 << p;
    const size_t n    = size << 1;

    indx.free();
    jndx.free();

    size_t nops = 0;

    register size_t j=1;
    for (register size_t i=1; i<n; i+=2)
    {
        if (j > i)
        {
            //core::bswap<2*sizeof(T)>( &arr[i], &arr[j] );
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


    ios::ocstream hdr("xbitrev.hpp",false);
    ios::ocstream src("xbitrev.cpp",false);

    hdr << "#ifndef YOCTO_XBITREV_INCLUDED\n";
    hdr << "#define YOCTO_XBITREV_INCLUDED 1\n\n";
    hdr << "#include \"yocto/code/bswap.hpp\"\n";
    hdr << "namespace yocto { namespace math {\n\n";

    src << "#include \"./xbitrev.hpp\"\n";
    src << "namespace yocto { namespace math {\n\n";

    hdr << "\tstruct xbitrev {\n";


    vector<size_t> sizes;
    vector<size_t> words;
    vector<size_t> indx;
    vector<size_t> jndx;

    for(size_t p=0;p<=pmax;++p)
    {
        const size_t sz = 1 << p;
        compute_swaps(p,indx,jndx);
        const size_t nw = indx.size();
        sizes.push_back(sz);
        words.push_back(nw);
        if(nw>0)
        {
            hdr("\t\tstatic const size_t indx%04u[%4u];\n",unsigned(sz),unsigned(nw));
            hdr("\t\tstatic const size_t jndx%04u[%4u];\n",unsigned(sz),unsigned(nw));

            src("\tconst size_t xbitrev::indx%04u[%4u]= {",unsigned(sz),unsigned(nw) );
            for(size_t i=1;i<=nw;++i)
            {
                if( 0 == ((i-1)&15) )
                {
                    src << "\n\t";
                }
                src("%4u", unsigned(indx[i]));
                if(i<nw)
                {
                    src << ',';
                }
            }

            src("\t};\n\n");

            src("\tconst size_t xbitrev::jndx%04u[%4u]= {",unsigned(sz),unsigned(nw) );
            for(size_t i=1;i<=nw;++i)
            {
                if( 0 == ((i-1)&15) )
                {
                    src << "\n\t";
                }
                src("%4u", unsigned(jndx[i]));
                if(i<nw)
                {
                    src << ',';
                }
            }

            src("\t};\n\n");
        }

    }

    // generic code
    hdr << "\n\t\ttemplate <typename T> static inline void run_safe( T arr[], const size_t size) throw() {\n";
    hdr << "\t\t\t const    size_t n = size << 1;\n";
    hdr << "\t\t\t register size_t j = 1;\n";
    hdr << "\t\t\t for(register size_t i=1; i<n; i+=2)\n";
    hdr << "\t\t\t {\n";
    hdr << "\t\t\t     if(j>i)\n";
    hdr << "\t\t\t     {\n";
    hdr << "\t\t\t         core::bswap<2*sizeof(T)>( &arr[i], &arr[j] );\n";
    hdr << "\t\t\t     }\n";
    hdr << "\t\t\t     register size_t m = size;\n";
    hdr << "\t\t\t     while( (m>=2) && (j>m) )\n";
    hdr << "\t\t\t     {\n";
    hdr << "\t\t\t        j -=  m;\n";
    hdr << "\t\t\t        m >>= 1;\n";
    hdr << "\t\t\t     }\n";
    hdr << "\t\t\t     j += m;\n";
    hdr << "\t\t\t }\n";
    hdr << "\t\t}\n\n";

    hdr << "\n\t\ttemplate <typename T> static inline void run( T arr[], const size_t size) throw() {\n";
    hdr << "\t\t\t\tswitch(size) {\n";

    for(size_t i=1;i<=sizes.size();++i)
    {
        const size_t sz = sizes[i];
        const size_t nw = words[i];
        hdr("\t\t\t\t case %4u:\n", unsigned(sz));
        if(nw>0)
        {
            hdr("\t\t\t\t\tfor(register size_t k=0;k<%4u;++k) { core::bswap<2*sizeof(T)>( &arr[indx%04u[k]], &arr[jndx%04u[k]] ); }\n", unsigned(nw), unsigned(sz), unsigned(sz) );
        }
        hdr << "\t\t\t\t\t  break;\n";
    }
    hdr << "\t\t\t\t default: run_safe(arr,size);\n";
    hdr << "\t\t\t\t};\n";
    hdr << "\t\t}\n\n";
    
    hdr << "\t};\n";
    
    hdr << "\n} }\n";
    src << "\n} }\n";
    hdr << "#endif\n";
    
    
}
YOCTO_PROGRAM_END()
