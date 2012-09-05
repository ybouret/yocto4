#include "yocto/utest/run.hpp"
#include "yocto/spade/types.hpp"
#include "yocto/spade/v1d.hpp"

using namespace yocto;
using namespace spade;

typedef geom::v1d<unit_t>   coord1D;

YOCTO_UNIT_TEST_IMPL(coord)
{

    coord1D C1(1);
    
    unit_t &x = __coord(C1, 0);
    std::cerr << "x=" << x << std::endl;
    x = 5;
    
    std::cerr << "x=" << C1.x << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
