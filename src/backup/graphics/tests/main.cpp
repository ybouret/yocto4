#include "yocto/utest/driver.hpp"

#include <cmath>
#include <cfloat>

YOCTO_UNIT_TEST_INIT(16)
{
    
    YOCTO_UNIT_TEST_DECL(bitmap);
    YOCTO_UNIT_TEST_DECL(pixmap);
    YOCTO_UNIT_TEST_DECL(img);
    YOCTO_UNIT_TEST_DECL(types);
    YOCTO_UNIT_TEST_DECL(ops);
    YOCTO_UNIT_TEST_DECL(tiff2png);
    YOCTO_UNIT_TEST_DECL(block);
    YOCTO_UNIT_TEST_DECL(tiff);
    YOCTO_UNIT_TEST_DECL(pa);
    YOCTO_UNIT_TEST_DECL(filter);
}
YOCTO_UNIT_TEST_EXEC()
