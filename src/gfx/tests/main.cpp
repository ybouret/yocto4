#include "yocto/utest/driver.hpp"

YOCTO_UNIT_TEST_INIT(16)
{
    YOCTO_UNIT_TEST_DECL(greyscale);
    YOCTO_UNIT_TEST_DECL(bitmap);
    YOCTO_UNIT_TEST_DECL(pixmap);
    YOCTO_UNIT_TEST_DECL(img);
    YOCTO_UNIT_TEST_DECL(tiff);
    YOCTO_UNIT_TEST_DECL(stencil);
}
YOCTO_UNIT_TEST_EXEC()
