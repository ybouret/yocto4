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


#include "yocto/geom/v2d.hpp"
typedef geom::v2d<double> vtx;

namespace
{
    class Param2
    {
    public:
        vtx tau;
        
        Param2()
        {
        }
        
        ~Param2()
        {
            
        }
        
        void get( array<double> &F, const array<double> &X )
        {
            const vtx a(X[1],X[2]);
            
            F[1] = a.norm2() - 3;
            F[2] = 1 - (a*tau)/a.norm();
        }
        
        void jac( matrix<double> &J, const array<double> &X )
        {
            const vtx a(X[1],X[2]);

            {
                array<double> &j = J[1];
                j[1] = 2*a.x; j[2] = 2*a.y;
            }
            
            {
                array<double> &j  = J[2];
                const double   as = a*tau;
                const double   an = a.norm();
                const double   an3 = an*an*an;
                j[1] = a.x * as / an3 - tau.x/an;
                j[2] = a.y * as / an3 - tau.y/an;
            }
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Param2);
    };
    
}

YOCTO_UNIT_TEST_IMPL(newton2)
{
    Param2                   p;
    Newton<double>::Function Fn( &p, & Param2::get );
    jacobian_of<double>      jwrapper(Fn);
    Newton<double>::Jacobian &Jn = jwrapper.call;
    vector<double>           X(2,0);
    
    p.tau.x = 1;
    p.tau.y = 1;
    p.tau.normalize();
    
    X[1] = 0.1;
    X[2] = 0.0;
    
    Newton<double>::solve(Fn, Jn, X, 1e-7);
    std::cerr << std::endl;
    std::cerr << "X=" << X << std::endl;
    
    vector<double> Y(2,0);
    Y[1] = 0.1;
    Y[2] = 0.0;
    Newton<double>::Jacobian Kn( &p, &Param2::jac);
    Newton<double>::solve(Fn, Kn, Y, 1e-7);
    std::cerr << std::endl;
    std::cerr << "X=" << X << std::endl;
    std::cerr << "Y=" << Y << std::endl;

}
YOCTO_UNIT_TEST_DONE()


