#include "yocto/utest/driver.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_INIT(16)
{
	YOCTO_UNIT_TEST_DECL(condition);
	YOCTO_UNIT_TEST_DECL(barrier);
	YOCTO_UNIT_TEST_DECL(crew);
	YOCTO_UNIT_TEST_DECL(SIMD);
}
YOCTO_UNIT_TEST_EXEC()

