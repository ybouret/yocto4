#include "yocto/utest/driver.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_INIT(8)
{
    YOCTO_UNIT_TEST_DECL(species);
    YOCTO_UNIT_TEST_DECL(collection);
    YOCTO_UNIT_TEST_DECL(equilibrium);
    YOCTO_UNIT_TEST_DECL(equilibria);
    YOCTO_UNIT_TEST_DECL(init);
}
YOCTO_UNIT_TEST_EXEC()

