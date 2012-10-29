#include "yocto/utest/run.hpp"
#include "yocto/math/opt/cgrad.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;
using namespace math;


inline
double Morse( double r )
{
    const double arg = 1 - exp( -r );
    return arg*arg;
}

inline
double dMorse( double r )
{
    const double eg = exp( -r );
    return 2 * (1-eg) * eg;
}

struct pot2
{
    double xa,xe,ya,ye;
    double func( const array<double> &X )
    {
        assert( X.size() == 2);
        const double x = X[1];
        const double y = X[2];
        const double dx = (x - xe)/xa;
        const double dy = (y - ye)/ya;
        const double r  = sqrt(dx*dx+dy*dy);
        //std::cerr << "func(" << x << "," << y << ")" << std::endl;
        return Morse( r );
    }

    
    void grad( array<double> &dFdX, const array<double> &X )
    {
        assert( X.size() == 2);
        const double x = X[1];
        const double y = X[2];
        //std::cerr << "grad(" << x << "," << y << ")" << std::endl;
        const double dx = (x - xe)/xa;
        const double dy = (y - ye)/ya;
        const double r  = sqrt(dx*dx+dy*dy);
        
        dFdX[1] = dx*dMorse(r)/xa;
        dFdX[2] = dy*dMorse(r)/ya;
    }
};

YOCTO_UNIT_TEST_IMPL(cgrad)
{
    pot2 P = { 0.3, 0.9, 0.4, 1.2 };
    numeric<double>::scalar_field Func( &P, &pot2::func );
    numeric<double>::vector_field Grad( &P, &pot2::grad );
    vector<double> X(2,0);
    X[1] = 0;
    X[2] = 0;
    cgrad(Func, Grad, X, 1e-7);
    std::cerr << "X=" << X << std::endl;
}
YOCTO_UNIT_TEST_DONE()
