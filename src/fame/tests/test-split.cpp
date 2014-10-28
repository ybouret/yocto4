#include "yocto/fame/split.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace fame;

YOCTO_UNIT_TEST_IMPL(split)
{
    const layout1D l1 = layout1D(-5,5);
    Links lx;
    for(int size=1;size<=6;++size)
    {
        std::cerr << std::endl;
        std::cerr << "size="  << size << " : full= " << l1 << std::endl;
        for(int rank=0;rank<size;++rank)
        {
            const layout1D l = Split(l1, rank, size,true,lx);
            std::cerr << "\trank="  << rank << " : " << l << ",\tlinks=" << lx << std::endl;
        }
    }
    
    const layout2D l2 = layout2D( coord2D(-5,-6), coord2D(5,6) );
    Links ly;
    
    for(int size=1; size<=16;++size)
    {
        std::cerr << std::endl;
        std::cerr << "size="  << size << " : full= " << l2 << std::endl;
        for(int rank=0;rank<1;++rank)
        {
            const layout2D l = Split(l2, rank, size, true, lx, true, ly);
        }
    }
    
    
    
}
YOCTO_UNIT_TEST_DONE()