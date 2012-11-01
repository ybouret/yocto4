#include "yocto/utest/run.hpp"
#include "yocto/math/fcn/newton.hpp"


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

YOCTO_UNIT_TEST_IMPL(newton)
{
    Param p;
    Newton<double>::Function Fn( &p, & Param::compute );
    jacobian_of<double>      jwrapper(Fn);
    Newton<double>::Jacobian &Jn = jwrapper.call;
    vector<double>           X(3,0);
    
    X[1] = 0.1;
    X[2] = 0.1;
    X[3] = 0.1;
    Newton<double>::solve(Fn, Jn, X, 1e-5);
    
}
YOCTO_UNIT_TEST_DONE()
