#include "yocto/utest/driver.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_INIT(16)
{
    YOCTO_UNIT_TEST_DECL(scanner);
    YOCTO_UNIT_TEST_DECL(lexer);
}
YOCTO_UNIT_TEST_EXEC()

