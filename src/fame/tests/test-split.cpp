#include "yocto/fame/split.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace fame;

YOCTO_UNIT_TEST_IMPL(split)
{
    const layout1D l1 = layout1D(-5,5);
    for(int size=1;size<=5;++size)
    {
        std::cerr << std::endl;
        std::cerr << "size="  << size << " : full= " << l1 << std::endl;
        for(int rank=0;rank<size;++rank)
        {
            const layout1D l = Link::Split(l1, rank, size);
            std::cerr << "\trank="  << rank << " : " << l << std::endl;
        }
    }
    
}
YOCTO_UNIT_TEST_DONE()