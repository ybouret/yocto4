#include "yocto/utest/run.hpp"
#include "yocto/parallel/split.hpp"

using namespace yocto;
using namespace parallel;

YOCTO_UNIT_TEST_IMPL(split)
{
    std::cerr << "sizeof(patch1D)=" << sizeof(patch1D) << std::endl;
    std::cerr << "sizeof(patch2D)=" << sizeof(patch2D) << std::endl;
    std::cerr << "sizeof(patch3D)=" << sizeof(patch3D) << std::endl;

    patch1D p1(1,100);
    patch1D p1b(p1);
    std::cerr << "p1 =" << p1  << std::endl;
    std::cerr << "p1b=" << p1b << std::endl;

    std::cerr << "\t(*) in 1D" << std::endl;
    for(size_t size=1;size<=8;++size)
    {
        std::cerr << std::endl << "#size=" << size << std::endl;
        for(size_t rank=0;rank<size;++rank)
        {
            std::cerr << "\t" << size << "." << rank << ":";
            const patch1D sub = split::compute(rank,size,p1);
            std::cerr << sub;
            std::cerr << std::endl;
        }
    }


    patch2D p2( coord2D(1,1), coord2D(20,20) );
    patch2D p2b(p2);

    std::cerr << "p2 =" << p2   << std::endl;
    std::cerr << "p2b=" << p2b  << std::endl;

    std::cerr << "\t(*) in 2D" << std::endl;
    for(size_t size=1;size<=8;++size)
    {
        std::cerr << std::endl << "#size=" << size << std::endl;
        const split::in2D s2(size,p2);
        std::cerr << "\txsize=" << s2.xsize << ", ysize=" << s2.ysize << std::endl;
        for(size_t rank=0;rank<size;++rank)
        {
            std::cerr << "\t" << size << "." << rank << ":";
            const patch2D sub = s2(rank);
            std::cerr << sub;
            std::cerr << std::endl;
        }

    }

}
YOCTO_UNIT_TEST_DONE()



