#include "yocto/utest/driver.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_INIT(16)
{
    
    YOCTO_UNIT_TEST_DECL(d1);
    YOCTO_UNIT_TEST_DECL(d2);
    YOCTO_UNIT_TEST_DECL(coll2D);
    YOCTO_UNIT_TEST_DECL(coll3D);
}
YOCTO_UNIT_TEST_EXEC()

