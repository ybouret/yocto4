#include "yocto/utest/run.hpp"
#include "yocto/container/vslot.hpp"


using namespace yocto;

YOCTO_UNIT_TEST_IMPL(vslot)
{
    vslot v1;
    
    v1.allocate(10);
    
}
YOCTO_UNIT_TEST_DONE()
