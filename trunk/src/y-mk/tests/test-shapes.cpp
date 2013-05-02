#include "yocto/utest/run.hpp"
#include "yocto/math/alg/shapes.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/math/types.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/string/conv.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;
using namespace math;


YOCTO_UNIT_TEST_IMPL(fit_circle)
{
    size_t n = 10;
    if(argc>1)
        n = strconv::to<size_t>(argv[1],"n");
    
    const double Xc = 10 + (alea<double>() - 0.5) * 20;
    const double Yc = 10 + (alea<double>() - 0.5) * 20;
    const double R  = 1  + 10*alea<double>();
    
    std::cerr << "Xc=" << Xc << std::endl;
    std::cerr << "Yc=" << Yc << std::endl;
    std::cerr << "R="  << R << std::endl;
    
    fit_circle<double> circ;
    
    {
        ios::ocstream fp("circ.dat",false);
        for( size_t i=1; i <= n; ++i )
        {
            const double theta = alea<double>() * numeric<double>::two_pi;
            const double X     = Xc+R * Cos(theta) + (0.5-alea<double>());
            const double Y     = Yc+R * Sin(theta) + (0.5-alea<double>());
            
            circ.append(X,Y);
            fp("%g %g\n",X,Y);
        }
    }
    
    std::cerr << "S=" << circ.__S() << std::endl;
    std::cerr << "Q=" << circ.__Q() << std::endl;
    
    v2d<double> C;
    double      radius=0;
    
    if( circ.solve(radius,C) )
    {
        std::cerr << "C=" << C << std::endl;
        std::cerr << "radius=" << radius << std::endl;
        ios::ocstream fp("fit_circ.dat",false);
        for( double theta=0; theta < numeric<double>::two_pi; theta += 0.01 )
        {
            fp("%g %g\n", C.x + radius*cos(theta), C.y + radius*sin(theta) );
        }
        fp("%g %g\n", C.x+radius,C.y);
    }
    else
        std::cerr << "Singular Distribution" << std::endl;
    
    

    
}
YOCTO_UNIT_TEST_DONE()


YOCTO_UNIT_TEST_IMPL(fit_ellipse)
{
    size_t n = 10;
    if(argc>1)
        n = strconv::to<size_t>(argv[1],"n");

    fit_ellipse<double> ell;
    
    const double Xc = 10 + (alea<double>() - 0.5) * 20;
    const double Yc = 10 + (alea<double>() - 0.5) * 20;

    const double Ra     = 4 + 2*alea<double>();
    const double Rb     = 1.5 + alea<double>();
    const double phi    =  alea<double>() * numeric<double>::two_pi;
    const double CosPhi = Cos(phi);
    const double SinPhi = Sin(phi);
    
    {
        ios::ocstream fp("ell.dat", false);
        
        for( size_t i=1; i <= n; ++i )
        {
            const double theta = alea<double>() * numeric<double>::two_pi;
            const double X     = Ra * Cos(theta);
            const double Y     = Rb * Sin(theta);
            const double x     = Xc + X*CosPhi - Y*SinPhi;
            const double y     = Yc + X*SinPhi + Y*CosPhi;
            fp("%g %g\n", x, y);
            ell.append(x,y);
        }
    }
    //std::cerr << "S=" << ell.__S() << std::endl;
    //std::cerr << "C=" << ell.__C() << std::endl;
    ell.solve();
    
}
YOCTO_UNIT_TEST_DONE()


