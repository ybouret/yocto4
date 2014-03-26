#include "yocto/utest/run.hpp"
#include "yocto/mpa/word2mpz.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(int)
{
    mpz x = -89;
    std::cerr << "x=" << x << std::endl;
    
    {
        mpa::word2mpz w( -512 );
        std::cerr << "z=" << w.z << std::endl;
    }
    
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
