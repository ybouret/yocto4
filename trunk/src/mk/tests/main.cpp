#include "yocto/utest/driver.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_INIT(64)
{
	YOCTO_UNIT_TEST_DECL(z);
	YOCTO_UNIT_TEST_DECL(matrix);
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
    YOCTO_UNIT_TEST_DECL(extend);

    YOCTO_UNIT_TEST_DECL(trigo);
    
    YOCTO_UNIT_TEST_DECL(svd);
	YOCTO_UNIT_TEST_DECL(bracket);
    YOCTO_UNIT_TEST_DECL(cgrad);
    YOCTO_UNIT_TEST_DECL(cgrad2);
    
    YOCTO_UNIT_TEST_DECL(newton);
    YOCTO_UNIT_TEST_DECL(newton2);
    YOCTO_UNIT_TEST_DECL(svdgs);
    
	YOCTO_UNIT_TEST_DECL(lu);
    YOCTO_UNIT_TEST_DECL(lu2);

    YOCTO_UNIT_TEST_DECL(tr2d);
    
    YOCTO_UNIT_TEST_DECL(fft1);
    YOCTO_UNIT_TEST_DECL(psd);
    YOCTO_UNIT_TEST_DECL(tridiag);
    YOCTO_UNIT_TEST_DECL(spline);
    YOCTO_UNIT_TEST_DECL(spline2d);

    YOCTO_UNIT_TEST_DECL(diag);
    
    YOCTO_UNIT_TEST_DECL(fit_circle);
    YOCTO_UNIT_TEST_DECL(fit_ellipse);

    YOCTO_UNIT_TEST_DECL(eigv);
    YOCTO_UNIT_TEST_DECL(balance);
    
    YOCTO_UNIT_TEST_DECL(michaelis);
    
    YOCTO_UNIT_TEST_DECL(ode_expl);

    
}
YOCTO_UNIT_TEST_EXEC()

