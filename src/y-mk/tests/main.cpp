#include "yocto/utest/driver.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_INIT(32)
{
	YOCTO_UNIT_TEST_DECL(z);
	YOCTO_UNIT_TEST_DECL(matrix);
	YOCTO_UNIT_TEST_DECL(linsys);
	YOCTO_UNIT_TEST_DECL(jacobi);
	
	YOCTO_UNIT_TEST_DECL(intg);
	YOCTO_UNIT_TEST_DECL(drvs);
	YOCTO_UNIT_TEST_DECL(zero);
	YOCTO_UNIT_TEST_DECL(opt);
	
	YOCTO_UNIT_TEST_DECL(ode);
	YOCTO_UNIT_TEST_DECL(lsf);
	YOCTO_UNIT_TEST_DECL(lsf2);
    
	YOCTO_UNIT_TEST_DECL(cholesky);
	YOCTO_UNIT_TEST_DECL(algebra);
    
    YOCTO_UNIT_TEST_DECL(stiff);
    YOCTO_UNIT_TEST_DECL(stat);
    
    YOCTO_UNIT_TEST_DECL(linear);
    YOCTO_UNIT_TEST_DECL(smooth);
    
    YOCTO_UNIT_TEST_DECL(trigo);
    
}
YOCTO_UNIT_TEST_EXEC()

