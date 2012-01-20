#include "yocto/utest/driver.hpp"

YOCTO_UNIT_TEST_INIT(8)
{
	YOCTO_UNIT_TEST_DECL(init);
	YOCTO_UNIT_TEST_DECL(ping);
	YOCTO_UNIT_TEST_DECL(sync);
}
YOCTO_UNIT_TEST_EXEC()
