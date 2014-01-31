#include "yocto/utest/run.hpp"
#include "yocto/math/fcn/intg.hpp"

using namespace yocto;
using namespace math;

namespace
{
    struct  functions
    {
        size_t count;
        
        double _tst( double x )
        {
            ++count;
            return Cos(x);
        }
    
        
        
    };
}

YOCTO_UNIT_TEST_IMPL(intg)
{
    functions fn;
    
    numeric<double>::function F( &fn, & functions::_tst );
    integrator<double>        intg;
    
    fn.count  = 0;
    double  s = 0;
    bool    flag = integrator<double>::quad(s, 0, numeric<double>::pi, F, 1e-7);
    std::cerr << "success = " << flag << std::endl;
    std::cerr << "counts  = " << fn.count << std::endl;
    std::cerr << "ans     = " << s << std::endl;
    
    fn.count = 0;
    s = intg(0,numeric<double>::pi,F,1e-9);
    
    std::cerr << "s=" << s << std::endl;
    std::cerr << "counts=" << fn.count << std::endl;
}
YOCTO_UNIT_TEST_DONE()

