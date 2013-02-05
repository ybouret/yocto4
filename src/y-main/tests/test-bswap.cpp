#include "yocto/utest/run.hpp"
#include "yocto/code/bswap.hpp"
#include "yocto/math/complex.hpp"

using namespace yocto;
using namespace math;


YOCTO_UNIT_TEST_IMPL(bswap)
{
    
    {
        char a = 'A';
        char b = 'B';
        bswap(a,b);
        std::cerr << a << ' ' << b << std::endl;
    }
    
    {
        short a = 0;
        short b = 1;
        bswap(a,b);
        std::cerr << a << ' ' << b << std::endl;
    }
    
    {
        int32_t a = 0;
        int32_t b = 1;
        bswap(a,b);
        std::cerr << a << ' ' << b << std::endl;
    }
    
    {
        float a = 0;
        float b = 1;
        bswap(a,b);
        std::cerr << a << ' ' << b << std::endl;
    }
    
    
    {
        double a = 0;
        double b = 1;
        bswap(a,b);
        std::cerr << a << ' ' << b << std::endl;
    }
    
    {
        complex<float> a(1,2);
        complex<float> b(3,4);
        bswap(a,b);
        std::cerr << a << ' ' << b << std::endl;
    }
    
    {
        complex<double> a(1,2);
        complex<double> b(3,4);
        bswap(a,b);
        std::cerr << a << ' ' << b << std::endl;
    }
    
}
YOCTO_UNIT_TEST_DONE()
