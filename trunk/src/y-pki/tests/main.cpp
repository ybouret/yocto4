#include "yocto/utest/driver.hpp"

YOCTO_UNIT_TEST_INIT(16)
{
	YOCTO_UNIT_TEST_DECL(rsa1);
	YOCTO_UNIT_TEST_DECL(rsa2);
    YOCTO_UNIT_TEST_DECL(auth);
    YOCTO_UNIT_TEST_DECL(actors);
}
YOCTO_UNIT_TEST_EXEC()

