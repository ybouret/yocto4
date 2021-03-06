#include "yocto/utest/driver.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_INIT(16)
{
    YOCTO_UNIT_TEST_DECL(console);
    YOCTO_UNIT_TEST_DECL(fame);
    
    YOCTO_UNIT_TEST_DECL(h1d);
    YOCTO_UNIT_TEST_DECL(h2d);
    YOCTO_UNIT_TEST_DECL(h3d);

}
YOCTO_UNIT_TEST_EXEC()

