#include "yocto/utest/driver.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_INIT(16)
{
    YOCTO_UNIT_TEST_DECL(console);
	YOCTO_UNIT_TEST_DECL(loop);
    YOCTO_UNIT_TEST_DECL(s2d);
    YOCTO_UNIT_TEST_DECL(silo);
    YOCTO_UNIT_TEST_DECL(s3d);
}
YOCTO_UNIT_TEST_EXEC()

