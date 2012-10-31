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
            assert(X.size()==2);
            assert(F.size()==X.size());
            const double x = X[1];
            const double y = X[2];
            
            F[1] = 9 - (x*x+y*y);
            F[2] = y-x;
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
    vector<double>           X(2,0);
    
    X[1] = 0.5;
    X[2] = 0.1;
    Newton<double>::solve(Fn, Jn, X, 1e-5);
    
}
YOCTO_UNIT_TEST_DONE()
