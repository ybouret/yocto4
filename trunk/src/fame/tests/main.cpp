#include "yocto/utest/driver.hpp"

YOCTO_UNIT_TEST_INIT(16)
{
    YOCTO_UNIT_TEST_DECL(layout);
    YOCTO_UNIT_TEST_DECL(array1d);
    YOCTO_UNIT_TEST_DECL(array2d);
    YOCTO_UNIT_TEST_DECL(array3d);
    YOCTO_UNIT_TEST_DECL(arrays);
    YOCTO_UNIT_TEST_DECL(box);
    YOCTO_UNIT_TEST_DECL(intg);
    YOCTO_UNIT_TEST_DECL(poisson);

}
YOCTO_UNIT_TEST_EXEC()


