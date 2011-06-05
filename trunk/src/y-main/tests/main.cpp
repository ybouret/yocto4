#include "yocto/utest/driver.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_INIT(16)
{
	YOCTO_UNIT_TEST_DECL(platform);
	YOCTO_UNIT_TEST_DECL(mutex);
	YOCTO_UNIT_TEST_DECL(round);
	YOCTO_UNIT_TEST_DECL(unroll);
	YOCTO_UNIT_TEST_DECL(wtime);
	YOCTO_UNIT_TEST_DECL(rand);
	
}
YOCTO_UNIT_TEST_EXEC()

