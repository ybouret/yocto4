#include "yocto/utest/driver.hpp"

YOCTO_UNIT_TEST_INIT(16)
{
    YOCTO_UNIT_TEST_DECL(layout);
    YOCTO_UNIT_TEST_DECL(array1d);
    YOCTO_UNIT_TEST_DECL(array2d);
    YOCTO_UNIT_TEST_DECL(array3d);
    YOCTO_UNIT_TEST_DECL(adb);
    YOCTO_UNIT_TEST_DECL(mesh);
    YOCTO_UNIT_TEST_DECL(vtk);
    YOCTO_UNIT_TEST_DECL(cell);

}
YOCTO_UNIT_TEST_EXEC()


