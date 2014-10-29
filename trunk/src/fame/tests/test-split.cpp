#include "yocto/fame/split/quad1d.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace fame;

#include <cstdlib>

YOCTO_UNIT_TEST_IMPL(split)
{
    const layout1D l1 = layout1D(-5,5);
    quad_links xlinks;
    
    for(int size=1;size<=8;++size)
    {
        std::cerr << std::endl;
        std::cerr << "size=" << size << ", full=" << l1 << std::endl;
        for(int rank=0;rank<size;++rank)
        {
            const layout1D s = quad_split(l1, rank, size, true,xlinks);
            std::cerr << "\trank=" << rank << ",\t sub=" << s << ",\t links=" << xlinks << std::endl;
        }
    }
    
    
}
YOCTO_UNIT_TEST_DONE()