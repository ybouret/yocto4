#include "yocto/utest/driver.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_INIT(16)
{
    YOCTO_UNIT_TEST_DECL(species);
    YOCTO_UNIT_TEST_DECL(collection);
    YOCTO_UNIT_TEST_DECL(solution);
    YOCTO_UNIT_TEST_DECL(validate);
    YOCTO_UNIT_TEST_DECL(equilibrium);
    YOCTO_UNIT_TEST_DECL(equilibria);
    YOCTO_UNIT_TEST_DECL(boot);
    YOCTO_UNIT_TEST_DECL(mix);
    YOCTO_UNIT_TEST_DECL(effector);
}
YOCTO_UNIT_TEST_EXEC()

