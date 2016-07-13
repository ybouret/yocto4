#include "yocto/utest/driver.hpp"

YOCTO_UNIT_TEST_INIT(16)
{
    YOCTO_UNIT_TEST_DECL(types);
    YOCTO_UNIT_TEST_DECL(pixmap);
    YOCTO_UNIT_TEST_DECL(ops);
    YOCTO_UNIT_TEST_DECL(tiff);
}
YOCTO_UNIT_TEST_EXEC()
