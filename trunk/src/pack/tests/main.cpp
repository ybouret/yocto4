#include "yocto/utest/driver.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_INIT(16)
{
	YOCTO_UNIT_TEST_DECL(huff);
    YOCTO_UNIT_TEST_DECL(huffenc);
    YOCTO_UNIT_TEST_DECL(huffdec);
    YOCTO_UNIT_TEST_DECL(comp);
}
YOCTO_UNIT_TEST_EXEC()

