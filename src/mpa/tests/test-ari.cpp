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
    

    for(int i=0;i<=20;++i)
    {
        const mpn n = mpn::factorial(i);
        std::cerr << i << "! = " << n << std::endl;
    }
    
    for(uint64_t n=1;n<=10;++n)
    {
        for(uint64_t p=0;p<=n;++p)
        {
            std::cerr << " " << mpn::binomial(n, p);
        }
        std::cerr << std::endl;
    }
    
    for(uint64_t n=1;n<=24;++n)
    {
        std::cerr << "n=" << n << std::endl;
        for(uint64_t p=0;p<=n;++p)
        {
            const mpn num_combi = mpn::binomial(n, p);
            const mpn num_items = p * num_combi;
            std::cerr << "\tp=" << p << " #combi=" << num_combi << " #items=" << num_items << std::endl;
        }
    }
    
}
YOCTO_UNIT_TEST_DONE()
