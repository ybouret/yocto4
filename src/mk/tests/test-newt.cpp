#include "yocto/utest/run.hpp"
#include "yocto/math/fcn/newt.hpp"


using namespace yocto;
using namespace math;

namespace
{
    class Param
    {
    public:

        Param()
        {
        }

        ~Param()
        {

        }

        bool callback( const array<double> &F, const array<double> &X)
        {
            std::cerr << "-- X=" << X << std::endl;
            std::cerr << "-- F=" << F << std::endl;
            std::cerr << std::endl;
            return true;
        }

        void compute( array<double> &F, const array<double> &X )
        {
            assert(X.size()==3);
            assert(F.size()==X.size());
            const double x = X[1];
            const double y = X[2];
            const double z = X[3];
            F[1] = 14 - (x*x+y*y+z*z);
            F[2] = y-x-1;
            const double dx  = x-0.1;
            F[3] = 2.19317121994613 - sqrt(dx*dx+y*y);
        }

        void jacob( matrix<double> &J, const array<double> &X)
        {
            assert(X.size()==3);
            assert(J.is_square());
            assert(J.cols==X.size());
            const double x = X[1];
            const double y = X[2];
            const double z = X[3];
            J[1][1] = -2*x; J[1][2] = -2*y; J[1][3] = -2*z;
            J[2][1] = -1;   J[2][2] = 1;    J[2][3] = 0;
            const double dx  = x-0.1;
            const double den = sqrt(dx*dx+y*y);
            J[3][1] = -dx/den; J[3][2] = -y/den; J[3][3] = 0;
        }

    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Param);
    };

}

#include "yocto/code/rand.hpp"

YOCTO_UNIT_TEST_IMPL(newt)
{
    Param p;
    newt<double>::Field    Fn( &p, & Param::compute );
    newt<double>::Jacobian Jn( &p, & Param::jacob   );
    vector<double>         X(3,0);

    X[1] = 10.0*(alea<double>()-0.5);
    X[2] = 10.0*(alea<double>()-0.5);
    X[3] = 10.0*(alea<double>()-0.5);

    vector<double> Y(X);
    vector<double> F(X.size(),0);
    newt<double>   newton;

    std::cerr << "Solving With Analytic Jacobian" << std::endl;

    if(newton.solve(Fn,Jn,X))
    {
        std::cerr << "X=" << X << std::endl;
        Fn(F,X);
        std::cerr << "F=" << F << std::endl;
    }

    std::cerr << "Solving With Numerical Jacobian" << std::endl;
    jacobian<double>       jac(Fn);
    newt<double>::Jacobian J1(&jac,&jacobian<double>::compute);

    if(newton.solve(Fn,J1,Y))
    {
        std::cerr << "Y=" << Y << std::endl;
        Fn(F,Y);
        std::cerr << "F=" << F << std::endl;
    }


}
YOCTO_UNIT_TEST_DONE()
