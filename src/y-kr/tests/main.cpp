#include "yocto/utest/driver.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_INIT(16)
{
	YOCTO_UNIT_TEST_DECL(hashing);
	YOCTO_UNIT_TEST_DECL(hmac);
	YOCTO_UNIT_TEST_DECL(sc);
	YOCTO_UNIT_TEST_DECL(bc);
    YOCTO_UNIT_TEST_DECL(rmd);
    YOCTO_UNIT_TEST_DECL(rmd160);
    YOCTO_UNIT_TEST_DECL(rmd128);
    YOCTO_UNIT_TEST_DECL(ios);

}
YOCTO_UNIT_TEST_EXEC()

