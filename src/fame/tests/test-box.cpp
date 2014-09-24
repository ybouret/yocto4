#include "yocto/utest/run.hpp"
#include "yocto/fame/box.hpp"


using namespace yocto;
using namespace fame;

YOCTO_UNIT_TEST_IMPL(box)
{
    
    box<1,float> b1(1,-1);
    std::cerr << "b1=" << b1 << std::endl;
    
    box<2,double> b2( math::v2d<double>(1,-2), math::v2d<double>(-1,3) );
    std::cerr << "b2=" << b2 << std::endl;

    box<3,double> b3( math::v3d<double>(1,-2,1.2), math::v3d<double>(-1,3,-0.7) );
    std::cerr << "b3=" << b3 << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()