#include "yocto/utest/run.hpp"
#include "yocto/mpa/natural.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(io)
{
    for(size_t i=0;i<10;++i)
    {
        const uint64_t x = _rand.full<uint64_t>();
        const mpn      n = x;
        const mpn      m = n;
        std::cerr << std::hex;
        std::cerr << "x=" << x << std::endl;
        std::cerr << "n=" << n << std::endl;
        std::cerr << "m=" << m << std::endl;
        const uint64_t y = m.to<uint64_t>();
        std::cerr << "y=" << y << std::endl;
        std::cerr << std::endl;
        
        if(x!=y) throw exception("invalid conversion");
    }
}
YOCTO_UNIT_TEST_DONE()
