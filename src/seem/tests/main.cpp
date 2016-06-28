#include "yocto/utest/driver.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_INIT(16)
{
    YOCTO_UNIT_TEST_DECL(parser);
    YOCTO_UNIT_TEST_DECL(eval);
    YOCTO_UNIT_TEST_DECL(compiler);
}
YOCTO_UNIT_TEST_EXEC()

