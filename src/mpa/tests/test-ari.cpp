#include "yocto/utest/run.hpp"
#include "yocto/mpa/natural.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(ari)
{
    {
        const mpn three = 3;
        const mpn eleven = 11;
        const mpn x      = mpn::mod_inv(three, eleven);
        std::cerr << "x= 0x" << x << std::endl;
        if( x.to<uint64_t>() != 4 )
        {
            throw exception("mod_inv error");
        }
    }
    
    {
        const mpn four     = 4;
        const mpn thirteen = 13;
        const mpn N        = 497;
        const mpn x        = mpn::mod_exp(four, thirteen, N);
        std::cerr << "x= 0x" << x << std::endl;
        if( x.to<uint64_t>() != 445 )
        {
            throw exception("mod_exp error");
        }
    }
    
    for(size_t i=1;i<=10;++i)
    {
        const mpn x = _rand.full<uint64_t>();
        mpn       y = _rand.full<uint64_t>();
        while( ! mpn::are_coprime(x, y) )
            ++y;
        std::cerr << x << " and " << y << " are coprime" << std::endl;
    }
    

}
YOCTO_UNIT_TEST_DONE()
