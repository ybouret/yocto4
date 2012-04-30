#include "yocto/utest/driver.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_INIT(16)
{
    YOCTO_UNIT_TEST_DECL(layout);
    YOCTO_UNIT_TEST_DECL(array);
    YOCTO_UNIT_TEST_DECL(arrays);
    YOCTO_UNIT_TEST_DECL(rmesh);

}
YOCTO_UNIT_TEST_EXEC()

