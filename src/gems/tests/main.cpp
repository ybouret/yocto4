#include "yocto/utest/driver.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_INIT(8)
{
	YOCTO_UNIT_TEST_DECL(xyz);
    YOCTO_UNIT_TEST_DECL(uuid);
}
YOCTO_UNIT_TEST_EXEC()

