#include "yocto/utest/run.hpp"
#include "yocto/math/ode/stiff-drvrs.hpp"
#include "yocto/math/ode/stiff-drvkr.hpp"

#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace math;

typedef double Real;

namespace  {
    
    
    class problem
    {
    public:
        problem() {}
        ~problem() throw() {}
        
        void derivs( array<Real> &dydx, Real x, const array<Real> &y )
        {
            dydx[1] = -0.013*y[1]-1000.0*y[1]*y[3];
            dydx[2] = -2500.0*y[2]*y[3];
            dydx[3] = -0.013*y[1]-1000.0*y[1]*y[3]-2500.0*y[2]*y[3];
        }
        
        
        void jacobn( array<Real> &dfdx, matrix<Real> &dfdy, Real x, const array<Real> &y)
        {
            for (size_t i=1;i<=dfdx.size();i++) dfdx[i]=0.0; 
            dfdy[1][1] = -0.013-1000.0*y[3]; 
            dfdy[1][2] =  0.0;
            dfdy[1][3] = -1000.0*y[1]; 
            dfdy[2][1] =  0.0;
            dfdy[2][2] = -2500.0*y[3];
            dfdy[2][3] = -2500.0*y[2];
            dfdy[3][1] = -0.013-1000.0*y[3]; 
            dfdy[3][2] = -2500.0*y[3];
            dfdy[3][3] = -1000.0*y[1]-2500.0*y[2];
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(problem);
    };
    
}



YOCTO_UNIT_TEST_IMPL(stiff)
{
    problem eq;
       
    ode::field<Real>::type derivs( &eq, & problem::derivs );
    ode::field<Real>::diff jacobn( &eq, & problem::jacobn );
    
   
    vector<Real> y(3,0.0);
    
    
    Real dx = 0.01;
    
    {
        ode::stiff_drvrs<Real>::type drv;
        drv.start(3);
        drv.eps = 1e-4;
        Real h  = 2.9e-4;
        y[1] = 1;
        y[2] = 1;
        y[3] = 0;
        
        ios::ocstream fp("stiff-rs.dat",false);
        fp("%g %g %g %g\n", 0.0, y[1], y[2], y[3] );
        for( size_t i=0; i < 1000; ++i )
        {
            Real x1 = i     * dx;
            Real x2 = (i+1) * dx;
            drv(derivs,jacobn,y,x1,x2,h);
            fp("%g %g %g %g\n", x2, y[1], y[2], y[3] );
        }
    }
    
    
    {
        ode::stiff_drvkr<Real>::type drv;
        drv.start(3);
        drv.eps = 1e-4;
        Real h  = 2.9e-4;
        y[1] = 1;
        y[2] = 1;
        y[3] = 0;
        
        ios::ocstream fp("stiff-kr.dat",false);
        fp("%g %g %g %g\n", 0.0, y[1], y[2], y[3] );
        for( size_t i=0; i < 1000; ++i )
        {
            Real x1 = i     * dx;
            Real x2 = (i+1) * dx;
            drv(derivs,jacobn,y,x1,x2,h);
            fp("%g %g %g %g\n", x2, y[1], y[2], y[3] );
        }
    }

    
}
YOCTO_UNIT_TEST_DONE()
