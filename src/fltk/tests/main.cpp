#include "yocto/utest/driver.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_INIT(16)
{
	YOCTO_UNIT_TEST_DECL(matte);
    YOCTO_UNIT_TEST_DECL(canvas);
}
YOCTO_UNIT_TEST_EXEC()

