#include "yocto/math/types.hpp"
#include "yocto/math/fcn/drvs.hpp"
#include "yocto/math/fcn/intg.hpp"
#include "yocto/exception.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/fs/vfs.hpp"

using namespace yocto;
using namespace math;

typedef numeric<double>::function Function;

class Fish
{
public:
    Function     W;
    Function     H;
    const double lambda;
    
    explicit Fish(const Function &userW,
                  const Function &userH,
                  const double    userlambda ) :
    W(userW),
    H(userH),
    lambda(userlambda),
    drvs(),
    drvs_h( 1e-4 ),
    intg(),
    dWs(this,& Fish::__dws ),
    dHs(this,& Fish::__dhs ),
    W_length( intg(0,1,dWs,1e-7) ),
    H_length( intg(0,1,dHs,1e-7) )
    {
        std::cerr << "W_length=" << W_length << std::endl;
        std::cerr << "H_length=" << H_length << std::endl;
    }
    
    virtual ~Fish() throw()
    {
        
    }
    
    
    
private:
    derivative<double> drvs;
    const double       drvs_h;
    integrator<double> intg;
    Function           dWs;
    Function           dHs;
    
    double __dws( double z )
    {
        return Hypotenuse(1,drvs(W,z,drvs_h));
    }
    
    double __dhs( double z )
    {
        return Hypotenuse(1,drvs(H,z,drvs_h));
    }
    
    double __perimeter(double z)
    {
        return 0;
    }
    
    YOCTO_DISABLE_COPY_AND_ASSIGN(Fish);
public:
    const double W_length;
    const double H_length;
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
        
        Fish F(WW,HH,0.1);
        
        return 0;
    }
    catch(...)
    {
        std::cerr << "Uncaught exception in " << program << std::endl;
    }
    return 1;
}
