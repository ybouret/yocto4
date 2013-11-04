#include "yocto/utest/driver.hpp"

YOCTO_UNIT_TEST_INIT(16)
{
    YOCTO_UNIT_TEST_DECL(bitmap);
    YOCTO_UNIT_TEST_DECL(pixmap);
    YOCTO_UNIT_TEST_DECL(rgb);
    YOCTO_UNIT_TEST_DECL(metrics);
    YOCTO_UNIT_TEST_DECL(format);
    YOCTO_UNIT_TEST_DECL(surface);
    
    YOCTO_UNIT_TEST_DECL(save);

}
YOCTO_UNIT_TEST_EXEC()

