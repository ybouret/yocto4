#include "yocto/utest/driver.hpp"


YOCTO_UNIT_TEST_INIT(16)
{
	YOCTO_UNIT_TEST_DECL(layout);
	YOCTO_UNIT_TEST_DECL(array);
	YOCTO_UNIT_TEST_DECL(split);
}
YOCTO_UNIT_TEST_EXEC()
