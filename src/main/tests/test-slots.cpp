#include "yocto/utest/run.hpp"
#include "yocto/container/slots.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(slots)
{
    slots_of<int> s(10);
    
}
YOCTO_UNIT_TEST_DONE()
