#include "yocto/utest/driver.hpp"

YOCTO_UNIT_TEST_INIT(16)
{
    YOCTO_UNIT_TEST_DECL(token);
    YOCTO_UNIT_TEST_DECL(source);
    YOCTO_UNIT_TEST_DECL(pattern);
    YOCTO_UNIT_TEST_DECL(regexp);
    YOCTO_UNIT_TEST_DECL(scanner);
}
YOCTO_UNIT_TEST_EXEC()
