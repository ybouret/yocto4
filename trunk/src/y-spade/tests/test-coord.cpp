#include "yocto/utest/run.hpp"
#include "yocto/spade/in1d.hpp"
#include "yocto/spade/in2d.hpp"
#include "yocto/spade/in3d.hpp"

using namespace yocto;
using namespace spade;


YOCTO_UNIT_TEST_IMPL(coord)
{

    coord1D C1(1);
    
    unit_t &x = __coord(C1, 0);
    std::cerr << "x=" << x << std::endl;
    x = 5;
    
    std::cerr << "x=" << C1 << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
