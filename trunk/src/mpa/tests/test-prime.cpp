#include "yocto/utest/run.hpp"
#include "yocto/mpa/natural.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(prime)
{
    mpn n;
    for(size_t i=0;i<100;++i,++n)
    {
        if( n.is_prime_() )
            std::cerr << n << std::endl;
    }
    
    n.ldz();
    for(size_t i=0;i<=20;++i,++n)
    {
        const mpn p = n.next_prime_();
        std::cerr << n << " => " << p << std::endl;
    }
    
    
}
YOCTO_UNIT_TEST_DONE()
