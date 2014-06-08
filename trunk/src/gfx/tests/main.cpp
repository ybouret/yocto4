#include "yocto/utest/driver.hpp"

YOCTO_UNIT_TEST_INIT(16)
{
    YOCTO_UNIT_TEST_DECL(bitmap);
    YOCTO_UNIT_TEST_DECL(pixmap);
    YOCTO_UNIT_TEST_DECL(format);
    YOCTO_UNIT_TEST_DECL(image);
    YOCTO_UNIT_TEST_DECL(load_jpeg);
    YOCTO_UNIT_TEST_DECL(load_png);

}
YOCTO_UNIT_TEST_EXEC()
