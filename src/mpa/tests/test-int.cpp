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
        std::cerr << "-512=" << w.z << std::endl;
    }
    
    {
        mpa::word2mpz w( -1 );
        std::cerr << "-1=" << w.z << std::endl;
    }
    
    for( int i=-3;i<=3;++i)
    {
        const mpz x = i;
        for(int j=-3;j<=3;++j)
        {
            const mpz y = j;
            const mpz sum = x+y;
            std::cerr << x << " + " << y << " = " << sum << std::endl;
            const mpz dif = x-y;
            std::cerr << x << " - " << y << " = " << dif << std::endl;
        }
        {
            mpz u = x;
            std::cerr << x << " , " << ++u << std::endl;
            mpz v = u++;
            std::cerr << v << " , " << u << std::endl;
        }
        {
            mpz u = x;
            std::cerr << x << " , " << --u << std::endl;
            mpz v = u--;
            std::cerr << v << " , " << u << std::endl;
        }
        std::cerr << "+ " <<x << " = " << +x << std::endl;
        std::cerr << "- " <<x << " = " << -x << std::endl;

    }
    
    
}
YOCTO_UNIT_TEST_DONE()
