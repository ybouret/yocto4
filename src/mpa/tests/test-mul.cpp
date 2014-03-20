#include "yocto/utest/run.hpp"
#include "yocto/mpa/natural.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(mul)
{
    for(size_t i=0;i<2000;++i)
    {
        const uint64_t l64 = _rand.full<uint32_t>();
        const uint64_t r64 = _rand.full<uint32_t>();
        const uint64_t p64 = l64 * r64;
        
        const mpn l = l64;
        const mpn r = r64;
        const mpn p = l*r;
        if( p.to<uint64_t>() != p64 )
            throw exception("mul error");
        
        const uint64_t sq64 = l64 * l64;
        const mpn      sq   = mpn::sqr(l);
        if( sq.to<uint64_t>() != sq64 )
            throw exception("sqr error");
    }
    
    mpn x = 1;
    std::cerr << x << std::endl;
    x = x * 3;
    std::cerr << x << std::endl;
    x = 2 * x;
    std::cerr << x << std::endl;
    x *= 4;
    std::cerr << x << std::endl;

}
YOCTO_UNIT_TEST_DONE()
