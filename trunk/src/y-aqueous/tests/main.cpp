#include "yocto/utest/driver.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_INIT(16)
{
    YOCTO_UNIT_TEST_DECL(species);
    YOCTO_UNIT_TEST_DECL(lib);
    YOCTO_UNIT_TEST_DECL(chemsys);
}
YOCTO_UNIT_TEST_EXEC()

