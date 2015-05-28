#include "yocto/sequence/vector.hpp"
#include "yocto/program.hpp"
#include "yocto/math/types.hpp"


using namespace yocto;
//using namespace math;

class Bridge
{
public:
    double  K;     //!< kappa*R
    double  beta;  //!< h/R
    double  theta; //!< angle in degrees !

    explicit Bridge( const double user_K, const double user_theta) :
    K(user_K),
    beta(0),
    theta(user_theta)
    {

    }


    inline ~Bridge() throw()
    {
    }

    
private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(Bridge);
};



YOCTO_PROGRAM_START()
{

}
YOCTO_PROGRAM_END()

