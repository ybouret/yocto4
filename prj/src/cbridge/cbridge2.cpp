#include "yocto/sequence/vector.hpp"
#include "yocto/sequence/many-arrays.hpp"
#include "yocto/program.hpp"
#include "yocto/math/types.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/math/kernel/tao.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace math;

static const size_t NVAR = 2;
static double       ATOL = 1e-4;
static double       BTOL = 1e-4;
static double       dY   = 1e-4;
typedef array<double> array_t;


class Bridge
{
public:

    double K;      //!< kappa * R
    double theta;  //!< wetting angle
    double beta;   //!< h/R
    many_arrays<double,memory::global> arrays;
    array_t &U;
    array_t &k1;
    array_t &k2;
    array_t &k3;
    array_t &k4;
    array_t &V;

    Bridge() :
    K(1),
    theta(0),
    beta(0),
    arrays(12),
    U(  arrays.next_array() ),
    k1( arrays.next_array() ),
    k2( arrays.next_array() ),
    k3( arrays.next_array() ),
    k4( arrays.next_array() ),
    V(  arrays.next_array() )
    {
        arrays.allocate(NVAR);
    }

    ~Bridge() throw()
    {
    }

    inline void Equation(array<double>       &dudy,
                         const double         y,
                         const array<double> &u) throw()
    {
        const double r     = u[1];
        const double drdy  = u[2];
        const double speed = Hypotenuse(1.0, drdy);
        const double accel = ((K*K*y)+1.0/r/speed)*speed*speed*speed;
        dudy[1] = drdy;
        dudy[2] = accel;
    }

    inline bool RK4(const double y_ini,
                    const double y_end) throw()
    {
        const double h     = y_end - y_ini;
        const double half  = h*0.5;
        const double y_mid = y_ini + half;
        Equation(k1,y_ini,U);

        tao::setprobe(V, U, half, k1);
        Equation(k2,y_mid,V);

        tao::setprobe(V,U,half,k2);
        Equation(k3,y_mid,V);

        tao::setprobe(V,U,h,k3);
        Equation(k4,y_end,V);

        for(size_t i=U.size();i>0;--i)
        {
            U[i] += h * (k1[i]+k2[i]+k2[i]+k3[i]+k3[i]+k4[i]) / 6.0;
        }

        return true; //IsValid(U[1],y_end);
    }

    inline void OutputBridge() const
    {
        const size_t  NB = 100;
        ios::ocstream fp("bridge.dat",false);
        for(size_t i=0;i<=NB;++i)
        {
            const double angle = (numeric<double>::two_pi*i)/NB;
            fp("%g %g\n",sin(angle),beta+(1.0-cos(angle)));
        }
    }

    inline void OutputTangents() const
    {
        ios::ocstream fp("tangents.dat",false);
        for(int alpha=0;alpha<=180;alpha+=15)
        {
            const double aa    = numeric<double>::pi * double(alpha)/180.0;
            const double angle = numeric<double>::pi * ( double(alpha)+theta) / 180.0;
            const double tr    = -cos(angle);
            const double tz    = -sin(angle);
            const double r_c   = sin(aa);
            const double z_c   = beta + (1-cos(aa));
            const double len   = 0.2;
            fp("%g %g\n%g %g\n\n",r_c,z_c,r_c+len*tr,z_c+len*tz);
        }

    }


private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(Bridge);
};

#include "yocto/string/conv.hpp"

YOCTO_PROGRAM_START()
{
    if(argc<=3)
    {
        throw exception("need K theta beta");
    }
    Bridge b;
    b.K     = strconv::to<double>(argv[1],"K");
    b.theta = strconv::to<double>(argv[2],"theta");
    b.beta  = strconv::to<double>(argv[3],"beta");

    b.OutputBridge();
    b.OutputTangents();

}
YOCTO_PROGRAM_END()
