#include "yocto/utest/driver.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_INIT(16)
{
	YOCTO_UNIT_TEST_DECL(platform);
	YOCTO_UNIT_TEST_DECL(context);
	YOCTO_UNIT_TEST_DECL(buffer);
	YOCTO_UNIT_TEST_DECL(kernel);
}
YOCTO_UNIT_TEST_EXEC()

