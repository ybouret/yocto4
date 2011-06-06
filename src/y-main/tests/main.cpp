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
	YOCTO_UNIT_TEST_DECL(thread);
	YOCTO_UNIT_TEST_DECL(pool);
	YOCTO_UNIT_TEST_DECL(list);
	YOCTO_UNIT_TEST_DECL(chunk);
	YOCTO_UNIT_TEST_DECL(at_exit);
	YOCTO_UNIT_TEST_DECL(singleton);
}
YOCTO_UNIT_TEST_EXEC()

