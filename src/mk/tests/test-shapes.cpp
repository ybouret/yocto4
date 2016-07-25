#include "yocto/utest/run.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/math/types.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/string/conv.hpp"
#include "yocto/math/alg/shapes2d.hpp"

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
    std::cerr << "R =" << R  << std::endl;

    FitCircle<double> fcd;
    FitCircle<float>  fcf;

    {
        ios::ocstream fp("circ.dat",false);
        for( size_t i=1; i <= n; ++i )
        {
            const double theta = alea<double>() * numeric<double>::two_pi;
            const double X     = Xc+R * Cos(theta) + (0.5-alea<double>());
            const double Y     = Yc+R * Sin(theta) + (0.5-alea<double>());

            fcd.append(X,Y);
            fcf.append(X,Y);
            fp("%g %g\n",X,Y);
        }
    }
    //std::cerr << "fcd.data=" << fcd.data << std::endl;
    point2d<double> cd;
    double          rd=0;

    fcd.compute(cd,rd);
    {
        std::cerr << "xc=" << cd.x << std::endl;
        std::cerr << "yc=" << cd.y << std::endl;
        std::cerr << "r =" << rd   << std::endl;

        {
            ios::ocstream fp("circ_fit.dat",false);
            for(double theta=0; theta <= 6.3; theta += 0.01 )
            {
                fp("%g %g\n", cd.x + rd * cos(theta), cd.y + rd * sin(theta) );
            }
        }
    }


    point2d<float> cf;
    float          rf=0;

    fcf.compute(cf,rf);
    {
        std::cerr << "xc=" << cf.x << std::endl;
        std::cerr << "yc=" << cf.y << std::endl;
        std::cerr << "r =" << rf   << std::endl;
    }


}
YOCTO_UNIT_TEST_DONE()

YOCTO_UNIT_TEST_IMPL(fit_ellipse)
{
    size_t n = 10;
    double noise = 0.1;
    if(argc>1)
        n = strconv::to<size_t>(argv[1],"n");


        const double Xc = 10 + (alea<double>() - 0.5) * 20;
        const double Yc = 10 + (alea<double>() - 0.5) * 20;

        const double Ra     = 4 + 2*alea<double>();
        const double Rb     = 1.5 + alea<double>();
        const double phi    = alea<double>() * numeric<double>::two_pi;
        const double CosPhi = Cos(phi);
        const double SinPhi = Sin(phi);

        std::cerr << "Xc=" << Xc << std::endl;
        std::cerr << "Yc=" << Yc << std::endl;
        std::cerr << "Ra=" << Ra << std::endl;
        std::cerr << "Rb=" << Rb << std::endl;


        FitConic<double> fcd;

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
            fcd.append(x,y);
        }
    }

        std::cerr << "Compute Generic..." << std::endl;
        vector<double> param(6);
        fcd.compute(FitConicGeneric,param);
        std::cerr << "param_generic=" << param << std::endl;

        std::cerr << "Compute Ellipse..." << std::endl;
        fcd.compute(FitConicEllipse,param);
        std::cerr << "param_ellipse=" << param << std::endl;

        std::cerr << "Reducing..." << std::endl;
        point2d<double> center, radius;
        matrix<double>  rotation(2,2);
        fcd.Reduce(center, radius, rotation, param);
        std::cerr << "center=" << center << std::endl;
        std::cerr << "radius=" << radius << std::endl;
        std::cerr << "rotation=" << rotation << std::endl;

    {
        ios::ocstream fp("ell_fit.dat",false);

        for(double theta=0; theta <= 6.3; theta += 0.1)
        {
            const double c  = Cos(theta);
            const double s  = Sin(theta);
            const point2d<double> R(radius.x*c,radius.y*s);
            point2d<double> v;
            tao::mul(v, rotation, R);
            v += center;
            fp("%g %g\n", v.x, v.y);
        }
    }

        }
        YOCTO_UNIT_TEST_DONE()

