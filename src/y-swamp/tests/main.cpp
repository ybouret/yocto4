#include "yocto/utest/driver.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_INIT(16)
{
    YOCTO_UNIT_TEST_DECL(layout);
    YOCTO_UNIT_TEST_DECL(array);
    YOCTO_UNIT_TEST_DECL(arrays);
    YOCTO_UNIT_TEST_DECL(rmesh);
    YOCTO_UNIT_TEST_DECL(cmesh);
    YOCTO_UNIT_TEST_DECL(data);
    YOCTO_UNIT_TEST_DECL(wksp);
    YOCTO_UNIT_TEST_DECL(split);
    YOCTO_UNIT_TEST_DECL(fill);
    YOCTO_UNIT_TEST_DECL(levels);
    YOCTO_UNIT_TEST_DECL(C2D);
    YOCTO_UNIT_TEST_DECL(C3D);
    YOCTO_UNIT_TEST_DECL(io);
    YOCTO_UNIT_TEST_DECL(delaunay);

}
YOCTO_UNIT_TEST_EXEC()

