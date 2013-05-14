#include "yocto/utest/driver.hpp"


using namespace yocto;


YOCTO_UNIT_TEST_INIT(16)
{
    YOCTO_UNIT_TEST_DECL(token);
    YOCTO_UNIT_TEST_DECL(source);
    YOCTO_UNIT_TEST_DECL(basic);
    YOCTO_UNIT_TEST_DECL(logic);
    YOCTO_UNIT_TEST_DECL(joker);
    YOCTO_UNIT_TEST_DECL(posix);

}
YOCTO_UNIT_TEST_EXEC()
