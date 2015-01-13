#include "yocto/utest/driver.hpp"

YOCTO_UNIT_TEST_INIT(16)
{
    YOCTO_UNIT_TEST_DECL(bitmap);
    YOCTO_UNIT_TEST_DECL(pixmap);
    YOCTO_UNIT_TEST_DECL(rawpix);
    YOCTO_UNIT_TEST_DECL(named);
    YOCTO_UNIT_TEST_DECL(image);
    YOCTO_UNIT_TEST_DECL(cluster);
    YOCTO_UNIT_TEST_DECL(filter);

}
YOCTO_UNIT_TEST_EXEC()
