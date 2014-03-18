#include "yocto/utest/driver.hpp"

YOCTO_UNIT_TEST_INIT(16)
{
    YOCTO_UNIT_TEST_DECL(mem);
    YOCTO_UNIT_TEST_DECL(io);
    YOCTO_UNIT_TEST_DECL(add);
    YOCTO_UNIT_TEST_DECL(cmp);
    YOCTO_UNIT_TEST_DECL(sub);
    YOCTO_UNIT_TEST_DECL(mul);
    YOCTO_UNIT_TEST_DECL(bits);
    YOCTO_UNIT_TEST_DECL(div);
    YOCTO_UNIT_TEST_DECL(ari);
    YOCTO_UNIT_TEST_DECL(parse);
    
}
YOCTO_UNIT_TEST_EXEC()
