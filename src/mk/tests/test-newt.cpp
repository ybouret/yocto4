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
            F[1] = 9 - (x*x+y*y+z*z);
            F[2] = y-x;
            F[3] = 1 - sqrt((x-0.1)*(x-0.1)+y*y);
        }

    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Param);
    };

}

#include "yocto/code/rand.hpp"

YOCTO_UNIT_TEST_IMPL(newt)
{
    Param p;
    newt<double>::field Fn( &p, & Param::compute );
    vector<double>      X(3,0);

    X[1] = 0.1+0.1*alea<double>();
    X[2] = 0.1+0.1*alea<double>();
    X[3] = 0.1+0.1*alea<double>();

    newt<double> newton;

    newton.solve(Fn,X);

}
YOCTO_UNIT_TEST_DONE()
