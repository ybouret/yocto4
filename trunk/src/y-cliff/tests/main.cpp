#include "yocto/utest/driver.hpp"


YOCTO_UNIT_TEST_INIT(16)
{
	YOCTO_UNIT_TEST_DECL(layout);
	YOCTO_UNIT_TEST_DECL(array);
	YOCTO_UNIT_TEST_DECL(split);
	YOCTO_UNIT_TEST_DECL(ppm);
	YOCTO_UNIT_TEST_DECL(comp);
	YOCTO_UNIT_TEST_DECL(wksp);
	YOCTO_UNIT_TEST_DECL(region);
	YOCTO_UNIT_TEST_DECL(contour2);
	YOCTO_UNIT_TEST_DECL(contour3);

}
YOCTO_UNIT_TEST_EXEC()
