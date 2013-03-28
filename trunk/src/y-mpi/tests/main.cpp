#include "yocto/utest/driver.hpp"

YOCTO_UNIT_TEST_INIT(16)
{
	YOCTO_UNIT_TEST_DECL(init);
	YOCTO_UNIT_TEST_DECL(ping);
	YOCTO_UNIT_TEST_DECL(sync);
	YOCTO_UNIT_TEST_DECL(bcast);
	YOCTO_UNIT_TEST_DECL(scatter);
    YOCTO_UNIT_TEST_DECL(string);
    YOCTO_UNIT_TEST_DECL(over);
    YOCTO_UNIT_TEST_DECL(comm);
}

YOCTO_UNIT_TEST_EXEC()
