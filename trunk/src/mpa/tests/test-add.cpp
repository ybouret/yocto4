#include "yocto/utest/run.hpp"
#include "yocto/mpa/natural.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(add)
{
    for(size_t i=0;i<10;++i)
    {
        const uint64_t l64 = _rand.full<uint32_t>();
        const uint64_t r64 = _rand.full<uint32_t>();
        const uint64_t s64 = l64 + r64;
        
        const mpn l = l64;
        const mpn r = r64;
        const mpn s = mpn::add(l,r);
        
        const uint64_t t64 = s.to<uint64_t>();
        
        if(t64!=s64)
            throw exception("add error");
    }
    
    mpn x; //!< zero
    for(size_t i=1;i<=1000000;++i)
    {
        ++x;
        if( x.to<size_t>() != i )
            throw exception("++ error");
    }
    
    std::cerr << std::dec;
    x  = uint64_t(0);
    std::cerr << x << std::endl;
    x += 2;
    std::cerr << x << std::endl;
    x  = 3 + x;
    std::cerr << x << std::endl;
    x  = x + 5;
    std::cerr << x << std::endl;

    
}
YOCTO_UNIT_TEST_DONE()