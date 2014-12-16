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

    
	YOCTO_UNIT_TEST_DECL(cholesky);
    
    YOCTO_UNIT_TEST_DECL(stiff);
    YOCTO_UNIT_TEST_DECL(stat);
    
    YOCTO_UNIT_TEST_DECL(linear);
    YOCTO_UNIT_TEST_DECL(extend);
    YOCTO_UNIT_TEST_DECL(extend2);

    YOCTO_UNIT_TEST_DECL(trigo);
    
    YOCTO_UNIT_TEST_DECL(svd);
    YOCTO_UNIT_TEST_DECL(ortho);
    
	YOCTO_UNIT_TEST_DECL(bracket);
    YOCTO_UNIT_TEST_DECL(cgrad);
    YOCTO_UNIT_TEST_DECL(cgrad2);
    
    YOCTO_UNIT_TEST_DECL(newton);
    YOCTO_UNIT_TEST_DECL(newton2);
    YOCTO_UNIT_TEST_DECL(svdgs);
    
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

    YOCTO_UNIT_TEST_DECL(crout);
    YOCTO_UNIT_TEST_DECL(mprove);
    

    YOCTO_UNIT_TEST_DECL(sparse);
    YOCTO_UNIT_TEST_DECL(spcj);
    
    YOCTO_UNIT_TEST_DECL(det);
    YOCTO_UNIT_TEST_DECL(tao);

	YOCTO_UNIT_TEST_DECL(gnl);
	YOCTO_UNIT_TEST_DECL(fit_poly);
	YOCTO_UNIT_TEST_DECL(fit_gauss);
    YOCTO_UNIT_TEST_DECL(fit_pade);
    YOCTO_UNIT_TEST_DECL(fit_vol);

    YOCTO_UNIT_TEST_DECL(grad);
    YOCTO_UNIT_TEST_DECL(extr);

    YOCTO_UNIT_TEST_DECL(bspline);
    
}
YOCTO_UNIT_TEST_EXEC()

