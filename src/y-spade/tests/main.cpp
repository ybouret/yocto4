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
    YOCTO_UNIT_TEST_DECL(fields);
    YOCTO_UNIT_TEST_DECL(data1D);
    YOCTO_UNIT_TEST_DECL(data2D);
    YOCTO_UNIT_TEST_DECL(data3D);
    YOCTO_UNIT_TEST_DECL(ghosts);
    YOCTO_UNIT_TEST_DECL(mesh);
    YOCTO_UNIT_TEST_DECL(region);
    YOCTO_UNIT_TEST_DECL(work1D);
    YOCTO_UNIT_TEST_DECL(work2D);
    YOCTO_UNIT_TEST_DECL(work3D);

}
YOCTO_UNIT_TEST_EXEC()

