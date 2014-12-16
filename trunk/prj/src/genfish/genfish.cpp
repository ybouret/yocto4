#include "yocto/math/types.hpp"
#include "yocto/math/fcn/drvs.hpp"
#include "yocto/math/fcn/intg.hpp"
#include "yocto/exception.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/fs/vfs.hpp"
#include "yocto/code/utils.hpp"

using namespace yocto;
using namespace math;

typedef numeric<double>::function Function;

class Fish
{
public:
    Function     W;
    Function     H;
    const size_t N;    //!< internal slices
    const double ftol;
    
    explicit Fish(const Function &userW,
                  const Function &userH,
                  const size_t    userN) :
    W(userW),
    H(userH),
    N(max_of<size_t>(userN,1)),
    ftol(1e-7),
    drvs(),
    drvs_h( 1e-4 ),
    intg(),
    dPerimeter(this,& Fish:: __dPerimeter),
    __a(0),
    __b(0),
    Perimeter(this, & Fish:: __perimeter ),
    Surface(this, & Fish:: __surface),
    TotalSurface( Surface(1.0) )
    {
    }
    
    virtual ~Fish() throw()
    {
        
    }
    
    
    
private:
    derivative<double> drvs;
    const double       drvs_h;
    integrator<double> intg;
    Function           dPerimeter;
    
    double __a;
    double __b;
    
    double __dPerimeter( double theta )
    {
        return Hypotenuse( __a * cos(theta), __b * cos(theta) );
    }
    
    double __perimeter(double z)
    {
        __a = W(z);
        __b = H(z);
        
        return intg(0,numeric<double>::two_pi,dPerimeter,ftol);
    }
    
    double __surface(double z)
    {
        if(z<=0)
            return 0;
        else
        {
            return intg(0,min_of<double>(z,1.0), Perimeter, ftol);
        }
    }
    
    YOCTO_DISABLE_COPY_AND_ASSIGN(Fish);
    
public:
    Function     Perimeter;
    Function     Surface; // cumulative surface
    const double TotalSurface;
    
};

static inline double H0(double z)
{
    return z*(1-z);
}

static inline double W0(double z)
{
    return sin(z*numeric<double>::pi);
}


int main(int argc, char *argv[] )
{
    static const char *program = vfs::get_base_name(argv[0]);
    try
    {
        Function WW = cfunctor(W0);
        Function HH = cfunctor(H0);
        
        Fish F(WW,HH,2);
        
        for(double z=0;z<=1;z+=0.1)
        {
            std::cerr << "Perimeter(" << z << ")=" << F.Perimeter(z) << std::endl;
            std::cerr << "Surface=" << F.Surface(z) << std::endl;
        }
        std::cerr << "TotalSurface=" << F.TotalSurface << std::endl;
        
        return 0;
    }
    catch(...)
    {
        std::cerr << "Uncaught exception in " << program << std::endl;
    }
    return 1;
}
