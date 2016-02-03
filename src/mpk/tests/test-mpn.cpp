#include "yocto/utest/run.hpp"
#include "yocto/mpk/natural.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace mpk;

YOCTO_UNIT_TEST_IMPL(mpn)
{
    natural_word w;
    std::cerr << "w=" << w << std::endl;
    w = 1;
    std::cerr << "w=" << w << std::endl;
    w = 0xABC;
    std::cerr << "w=" << w << std::endl;
    
    word_t n64 = _rand.full<word_t>();
    std::cerr << "n64=" << std::hex << n64 << std::dec << std::endl;
    natural n = n64;
    std::cerr << "n=" << n << std::endl;
    natural m = +n;
    std::cerr << "m=" << m << std::endl;
    
    m=1;
    std::cerr << "m=" << m << std::endl;
}
YOCTO_UNIT_TEST_DONE()
