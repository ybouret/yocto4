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
    {
        n = strconv::to<size_t>(argv[1],"n");
    }

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
            const double weight = 1+0.5*alea<double>();
            fcd.append(X,Y,weight);
            fcf.append(float(X),float(Y));
            fp("%g %g\n",X,Y);
        }
    }
    //std::cerr << "fcd.data=" << fcd.data << std::endl;
    point2d<double> cd;
    double          rd=0;

    fcd.compute(cd,rd);
    {
        std::cerr << "\nweighted double..." << std::endl;
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
        std::cerr << "\nunweighted float..." << std::endl;
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
    {
        n = strconv::to<size_t>(argv[1],"n");
    }

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
    FitConic<float>  fcf;

    double xmin = Xc;
    double xmax = Xc;
    double ymin = Yc;
    double ymax = Yc;
    vector<double> XX(n,as_capacity);
    vector<double> YY(n,as_capacity);

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
            xmin = min_of(xmin,x);
            xmax = max_of(xmax,x);
            ymin = min_of(ymin,y);
            ymax = max_of(ymax,y);


            const double weight = 1+0.5*alea<double>();
            fcd.append(x,y);
            fcf.append(float(x),float(y),float(weight));
            XX.push_back(x);
            YY.push_back(y);
        }
    }

    std::cerr << "Compute Generic..." << std::endl;
    vector<double> Ag(6);
    fcd.compute(FitConicGeneric,Ag);
    std::cerr << "param_generic=" << Ag << std::endl;


    std::cerr << "Compute Ellipse..." << std::endl;
    vector<double> Ae(6);
    fcd.compute(FitConicEllipse,Ae);
    std::cerr << "param_ellipse=" << Ae << std::endl;

    {
        ios::wcstream fpg("fit_g.dat");
        ios::wcstream fpe("fit_e.dat");
        const double dx  = xmax-xmin;
        const double dy  = ymax-ymin;
        const size_t NP  = 50;
        const double vcg = FitConic<double>::Eval(Ag,Xc,Yc);
        const double vce = FitConic<double>::Eval(Ae,Xc,Yc);
        for(size_t i=0;i<=NP;++i)
        {
            const double x = xmin + (dx*i)/NP;
            for(size_t j=0;j<=NP;++j)
            {
                const double y  = ymin + (dy*j)/NP;
                const double vg = FitConic<double>::Eval(Ag,x,y);
                const double ve = FitConic<double>::Eval(Ae,x,y);
                if(vg*vcg>=0)
                {
                    fpg("%g %g\n",x,y);
                }
                if(ve*vce>=0)
                {
                    fpe("%g %g\n",x,y);
                }
            }
        }
    }

    std::cerr << "Reducing..." << std::endl;
    point2d<double> center, radius;
    matrix<double>  rotation(2,2);
    FitConic<double>::Reduce(center, radius, rotation, Ae);
    std::cerr << "center=" << center << std::endl;
    std::cerr << "radius=" << radius << std::endl;
    std::cerr << "rotate=" << rotation << std::endl;
    
    {
        ios::wcstream fp("ell_center.dat");
        for(size_t i=1;i<=n;++i)
        {
            fp("%g %g\n%g %g\n\n", center.x, center.y,XX[i],YY[i]);
        }
    }


    {
        ios::wcstream fp("ell_fit.dat");
        for(double theta=0;theta<=6.3;theta+=0.01)
        {
            const double c  = cos(theta);
            const double s  = sin(theta);
            const point2d<double> rr(radius.x*c,radius.y*s);
            point2d<double> v;
            tao::mul(v, rotation, rr);
            v += center;
            fp("%g %g\n", v.x, v.y);
        }
    }
    

    }
    YOCTO_UNIT_TEST_DONE()

