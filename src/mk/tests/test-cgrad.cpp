#include "yocto/utest/run.hpp"
#include "yocto/math/opt/cgrad.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ios/ocstream.hpp"

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
    
    bool cb( const array<double> &X  )
    {
        ios::ocstream fp("cgrad.dat",true);
        std::cerr << "\t" << X << std::endl;
        fp("%g %g\n", X[1], X[2]);
        return true;
    }
};

#include "yocto/string/conv.hpp"
YOCTO_UNIT_TEST_IMPL(cgrad)
{
    
    double ftol = 1e-7;
    if( argc > 1 )
    {
        ftol = strconv::to<double>(argv[1],"ftol");
    }
    
    pot2 P = { 0.3, 0.9, 0.4, 1.2 };
    numeric<double>::scalar_field Func( &P, &pot2::func );
    numeric<double>::vector_field Grad( &P, &pot2::grad );
    vector<double> X(2,0);
    X[1] = 0;
    X[2] = 0;
    { ios::ocstream fp("cgrad.dat",false); }
    
    cgrad<double>::callback cb( &P, &pot2::cb);
    cgrad<double>::optimize(Func, Grad, X, ftol, &cb);
    std::cerr << "X=" << X << std::endl;
    
    X[1] = 0;
    X[2] = 0;
    vector<double> dX( X.size(), 1e-4 );
    cgrad<double> cg;
    cg.run(Func, X, dX, ftol, &cb);
    std::cerr << "X=" << X << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/math/v2d.hpp"
typedef v2d<double> vtx;

struct Param
{
    vtx    tau;
    double len;
    
    inline Param() : tau(), len(0) {}
    
    double func( const array<double> &var )
    {
        assert( var.size() == 2);
        const vtx a( var[1], var[2]);
        const double an = a.norm();
        const double as = a*tau;
        const double F1 = 1 - as/an;
        
        const double dl = (an - len)/len;
        const double F2 = 0.5*dl*dl;
        const double ans = F1 + F2;
        //std::cerr << "func(" << var << ")=" << ans << std::endl;
        return ans;
    }
    
    void grad( array<double> &g, const array<double> &var )
    {
        assert( var.size() == 2);
        assert( var.size() == 2);
        const vtx a( var[1], var[2]);
        const double an = a.norm();
        const double as = a*tau;
        const double dl = (an - len)/len;
        
        g[1] = a.x * as / (an*an*an) - tau.x/an + a.x * dl / (an*len);
        g[2] = a.y * as / (an*an*an) - tau.y/an + a.y * dl / (an*len);
        
        std::cerr << "grad(" << var << ")=" << g << std::endl;
    }
    
    
    bool cb( const array<double> &var )
    {
        std::cerr << "func(" << var << ")=" << func(var) << std::endl;
        //fgetc(stdin);
        ios::ocstream fp("cgrad2.dat",true);
        fp("%g %g\n", var[1], var[2]);
        return true;
    }
};


YOCTO_UNIT_TEST_IMPL(cgrad2)
{
    Param param;
    param.tau.x = 1;
    param.tau.y = 1;
    param.tau.normalize();
    param.len   = 1.2;
    
    vector<double> var(2,0);
    var[1] =  0.5;
    var[2] = -0.3;
    
    numeric<double>::scalar_field Func( &param, &Param::func );
    numeric<double>::vector_field Grad( &param, &Param::grad );
    std::cerr << "Func(" << var << ")=" << Func(var) << std::endl;
    //std::cerr << "Grad(" << var << ")=" << Grad(var) << std::endl;
    
    { ios::ocstream fp("cgrad2.dat",false); }
    
    cgrad<double>::callback cb( &param, &Param::cb );
    cgrad<double>::optimize(Func,Grad,var,1e-5,&cb);
    std::cerr << "var=" << var << std::endl;
}
YOCTO_UNIT_TEST_DONE()


