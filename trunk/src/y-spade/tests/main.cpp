#include "yocto/utest/driver.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_INIT(16)
{
    YOCTO_UNIT_TEST_DECL(coord);
    YOCTO_UNIT_TEST_DECL(layout);
    YOCTO_UNIT_TEST_DECL(array1D);
    YOCTO_UNIT_TEST_DECL(array2D);
    YOCTO_UNIT_TEST_DECL(array3D);
    YOCTO_UNIT_TEST_DECL(arrays);
}
YOCTO_UNIT_TEST_EXEC()

