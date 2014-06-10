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
    
    
    v2d<double> C;
    double      radius=0;
    circ.solve(radius,C);
    std::cerr << "C=" << C << std::endl;
    std::cerr << "radius=" << radius << std::endl;
    ios::ocstream fp("fit_circ.dat",false);
    for( double theta=0; theta < numeric<double>::two_pi; theta += 0.01 )
    {
        fp("%g %g\n", C.x + radius*cos(theta), C.y + radius*sin(theta) );
    }
    fp("%g %g\n", C.x+radius,C.y);
    
}
YOCTO_UNIT_TEST_DONE()


YOCTO_UNIT_TEST_IMPL(fit_ellipse)
{
    size_t n = 10;
    double noise = 0.1;
    if(argc>1)
        n = strconv::to<size_t>(argv[1],"n");
    
    fit_conic<double> ell;

    const double Xc = 10 + (alea<double>() - 0.5) * 20;
    const double Yc = 10 + (alea<double>() - 0.5) * 20;
    
    const double Ra     = 4 + 2*alea<double>();
    const double Rb     = 1.5 + alea<double>();
    const double phi    = alea<double>() * numeric<double>::two_pi;
    const double CosPhi = Cos(phi);
    const double SinPhi = Sin(phi);
    
    {
        ios::ocstream fp("ell.dat", false);
        
        for( size_t i=1; i <= n; ++i )
        {
            const double theta = alea<double>() * numeric<double>::two_pi;
            const double X     = Ra * Cos(theta);
            const double Y     = Rb * Sin(theta);
            const double x     = Xc + X*CosPhi - Y*SinPhi + (0.5 - alea<double>()) * noise;
            const double y     = Yc + X*SinPhi + Y*CosPhi + (0.5 - alea<double>()) * noise;;
            fp("%g %g\n", x, y);
            ell.append(x,y);
        }
    }
    vector<double> param(6,0);
    v2d<double>    radius,center;
    m2d<double>    rot;
    
    std::cerr << "Ra=" << Ra << std::endl;
    std::cerr << "Rb=" << Rb << std::endl;
    std::cerr << "Xc=" << Xc << std::endl;
    std::cerr << "Yc=" << Yc << std::endl;
    std::cerr << "Cos(Phi)=" << CosPhi << std::endl;
    std::cerr << "Sin(Phi)=" << SinPhi << std::endl;
    std::cerr << "#Ellipse:" << std::endl;
    ell.solve(conic_ellipse,param);
    std::cerr << "param=" << param << std::endl;
    ell.reduce(center,radius,rot,param);
    std::cerr << "ellipse:" << std::endl;
    std::cerr << "center=" << center << std::endl;
    std::cerr << "radius=" << radius << std::endl;
    std::cerr << "rot=" << rot << std::endl;
    {
        ios::ocstream fp("ell_fit.dat",false);
        for(double theta=0; theta <= 6.3; theta += 0.1)
        {
            const double c = Cos(theta);
            const double s = Sin(theta);
            const double Rx = radius.x * c;
            const double Ry = radius.y * s;
            const double x  = center.x + Rx * rot.ex.x + Ry * rot.ey.x;
            const double y  = center.y + Rx * rot.ex.y + Ry * rot.ey.y;
            fp("%g %g\n", x, y);
        }
    }
    
    std::cerr << std::endl;
    std::cerr << "#Generic:" << std::endl;
    ell.solve(conic_generic,param);
    std::cerr << "param=" << param << std::endl;
    ell.reduce(center,radius,rot,param);
    std::cerr << "ellipse:" << std::endl;
    std::cerr << "center=" << center << std::endl;
    std::cerr << "radius=" << radius << std::endl;
    std::cerr << "rot=" << rot << std::endl;
    {
        ios::ocstream fp("gen_fit.dat",false);
        for(double theta=0; theta <= 6.3; theta += 0.1)
        {
            const double c = Cos(theta);
            const double s = Sin(theta);
            const double Rx = radius.x * c;
            const double Ry = radius.y * s;
            const double x  = center.x + Rx * rot.ex.x + Ry * rot.ey.x;
            const double y  = center.y + Rx * rot.ex.y + Ry * rot.ey.y;
            fp("%g %g\n", x, y);
        }

    }
   
    
}
YOCTO_UNIT_TEST_DONE()

