#include "yocto/utest/driver.hpp"


using namespace yocto;


YOCTO_UNIT_TEST_INIT(8)
{
	YOCTO_UNIT_TEST_DECL(require);
    YOCTO_UNIT_TEST_DECL(tables);
    
}
YOCTO_UNIT_TEST_EXEC()

