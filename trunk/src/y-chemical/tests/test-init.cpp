#include "yocto/utest/run.hpp"
#include "yocto/chemical/initializer.hpp"


using namespace yocto;


YOCTO_UNIT_TEST_IMPL(init)
{
    chemical::initializer ini;
    
    chemical::constraint  &C1 = ini.create(1);
    C1["Na+"] = 1;
}
YOCTO_UNIT_TEST_DONE()

