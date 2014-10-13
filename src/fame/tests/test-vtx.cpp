#include "yocto/utest/run.hpp"
#include "yocto/fame/vertices.hpp"


using namespace yocto;
using namespace fame;
using namespace math;


YOCTO_UNIT_TEST_IMPL(vtx)
{
    vertices<1,float>  v1f("v1f", layout1D(-5,5) );
    vertices<2,double> v2d("v2d", layout1D(1,10) );
    vertices<3,double> v3d("v3d", layout1D(1,20) );
    
}
YOCTO_UNIT_TEST_DONE()
