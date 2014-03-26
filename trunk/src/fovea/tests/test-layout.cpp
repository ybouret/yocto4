#include "yocto/utest/run.hpp"
#include "yocto/fovea/dimensions.hpp"

using namespace yocto;
using namespace fovea;

YOCTO_UNIT_TEST_IMPL(layout)
{
    layout1D L1(-10,10);
    std::cerr << "L1=" << L1 << std::endl;
    
    layout2D L2( coord2D(-5,-5), coord2D(6,6) );
    std::cerr << "L2=" << L2 << std::endl;

    layout3D L3( coord3D(-5,-5,-5), coord3D(6,6,6) );
    std::cerr << "L3=" << L3 << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()

