#include "yocto/utest/run.hpp"
#include "yocto/mpa/integer.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(int)
{
    mpz x = -89;
    std::cerr << "x=" << x << std::endl;
    
    for( int i=-5;i<=5;++i)
    {
        const mpz x = i;
        for(int j=-5;j<=5;++j)
        {
            const mpz y = j;
            const mpz sum = x+y;
            std::cerr << x << " + " << y << " = " << sum << std::endl;
        }
    }
    
}
YOCTO_UNIT_TEST_DONE()
