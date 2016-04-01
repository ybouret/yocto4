#include "yocto/math/ode/rk4.hpp"
#include "yocto/utest/run.hpp"
//#include "yocto/math/ode/explicit/driver-ck.hpp"

using namespace yocto;
using namespace math;

namespace
{

    class dummy
    {
    public:
        dummy()
        {
        }

        ~dummy() throw()
        {
        }

        void eq1( array<double> &dYdx, double, const array<double> &Y)
        {
            assert(Y.size()>=1);
            dYdx[1] = -0.4 * Y[1];
        }

        void cb1( array<double> &Y, double )
        {
            if(Y[1]<=0) Y[1]=0;
        }

        void eq2( array<double> &dYdx, double, const array<double> &Y)
        {
            assert(Y.size()>=2);
            dYdx[1]  = Y[2];
            dYdx[2]  = -4.0*Y[1];
        }


    private:

    };

}

#include "yocto/ios/ocstream.hpp"

YOCTO_UNIT_TEST_IMPL(rk4)
{
    ode::RK4<double>            rk4;
    //ode::driverCK<double>::type rkck(1e-7);

    dummy dum;


    {
        ode::Field<double>::Equation eq1( &dum, & dummy::eq1);
        ode::Field<double>::Callback cb1( &dum, & dummy::cb1);
        ios::wcstream fp("rk4_eq1.dat");
        rk4.allocate(1);
        array<double> &Y = rk4.YY;
        Y[1] = 1;
        const double dx=0.01;
        fp("0 %g\n", Y[1]);
        for(double x=0;x<=1;x+=dx)
        {
            const double x1=x+dx;
            rk4(eq1,Y,x,x1,&cb1);
            fp("%g %g\n", x1, Y[1]);
        }
    }


    {
        ios::wcstream fp("rk4_eq2.dat");
        ode::Field<double>::Equation eq2( &dum, & dummy::eq2);
        rk4.allocate(2);
        array<double> &Y = rk4.YY;
        Y[1] = 1;
        Y[2] = 0;
        const double dx=0.01;
        for(double x=0;x<=6;x+=dx)
        {
            const double x1=x+dx;
            rk4(eq2,Y,x,x1,NULL);
            fp("%g %g\n", x1, Y[1]);
        }

    }
    
}
YOCTO_UNIT_TEST_DONE()

