#include "yocto/math/dat/spline.hpp"
#include "yocto/program.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-maths.hpp"
#include "yocto/math/v3d.hpp"
#include "yocto/math/fcn/drvs.hpp"
#include "yocto/math/fcn/intg.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace math;

typedef numeric<double>::function Function;
typedef v3d<double>               vtx_t;

#define FTOL 1e-4

class Fish
{
public:
    Function Width;
    Function Up;
    Function Down;
    Function Mix;

    Function X;          //!< for ztmp
    Function Y;          //!< for ztmp
    Function dPerimeter; //!< for ztmp
    Function Perimeter;  //!< function of z
    const size_t N;      //!< internal slices
    const size_t NZ;     //!< N+2

    inline Fish(const Function &userWidth,
                const Function &userUp,
                const Function &userDown,
                const Function &userMix,
                const size_t   userN
                ) :
    Width( userWidth ),
    Up(userUp),
    Down(userDown),
    Mix(userMix),
    X( this, & Fish:: __X ),
    Y( this, & Fish:: __Y ),
    dPerimeter(this, & Fish::__dPerimeter),
    Perimeter(this,  & Fish::__Perimeter),
    N( max_of<size_t>(1,userN) ),
    NZ(N+2),

    drvs(),
    drvs_h(1e-4),
    ztmp(0),
    intp(),
    zs(N+2,as_capacity),
    vs(N+2,as_capacity)
    {
        //______________________________________________________________________
        //
        // evaluate surface
        //______________________________________________________________________
        const double delta = 1.0 / (N+1);
        zs.push_back(0);
        vs.push_back(0);
        std::cerr << "-- Computing Surface..." << std::endl;
        for(size_t i=1;i<=N;++i)
        {
            std::cerr << "\t " << i << "/ " << N << "        \r"; std::cerr.flush();
            const double znew = i*delta;
            const double snew = vs.back() + Surface(zs.back(), znew);
            zs.push_back(znew);
            vs.push_back(snew);
        }
        std::cerr << std::endl;
        std::cerr << "-- Final Surface..." << std::endl;
        {
            const double znew = 1.0;
            const double snew = vs.back() + Surface(zs.back(), znew);
            zs.push_back(znew);
            vs.push_back(snew);
        }
        std::cerr << "\tS=" << vs.back() << std::endl;

        {
            ios::ocstream fp("surface.dat",false);
            for(size_t i=1;i<=NZ;++i)
            {
                fp("%g %g\n", zs[i], vs[i]);
            }
        }

    }

    inline ~Fish() throw()
    {
    }

    derivative<double> drvs;
    double             drvs_h;
    double             ztmp;
    integrator<double> intp;
    integrator<double> ints;
    vector<double>     zs; //!< z for surface
    vector<double>     vs; //!< value of surface


    inline double Surface(double z1,double z2)
    {
        return ints(z1,z2,Perimeter,FTOL);
    }

private:

    YOCTO_DISABLE_COPY_AND_ASSIGN(Fish);

    inline double __X( double theta )
    {
        return Width(ztmp) * cos(theta);
    }

    inline double __Y( double theta )
    {
        const double u = Up(ztmp);
        const double d = Down(ztmp);
        const double ratio = 0.5*(1+sin(theta));
        const double m     = Mix(ratio);
        return (u*m+d*(1-m))*sin(theta);
    }

    inline double __dPerimeter( double theta )
    {
        const double dX = drvs(X,theta,FTOL);
        const double dY = drvs(Y,theta,FTOL);
        return Hypotenuse(dX,dY);
    }

    inline double __Perimeter( double z )
    {
        ztmp = z;
        return intp(0.0,numeric<double>::two_pi,dPerimeter,FTOL);
    }



};


YOCTO_PROGRAM_START()
{
    Lua::State VM;
    if(argc<=1)
    {
        throw exception("need a config.lua");
    }

    lua_State *L = VM();
    Lua::Config::DoFile(L,argv[1]);
    Lua::Function<double> Width(L,"Width",true);
    Lua::Function<double> Up(L,"Up",true);
    Lua::Function<double> Down(L,"Down",true);
    Lua::Function<double> Mix(L,"Mix",true);
    const size_t N = size_t(Lua::Config::Get<lua_Number>(L, "N"));


    Fish fish(Width,Up,Down,Mix,N);

}
YOCTO_PROGRAM_END()

