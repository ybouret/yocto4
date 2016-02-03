#include "yocto/utest/run.hpp"
#include "yocto/mpk/word2mpn.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace mpk;

#define _SHOW(X) std::cerr << #X " = " << X << std::endl

YOCTO_UNIT_TEST_IMPL(mpn)
{
    {
        natural  n0;    _SHOW(n0);
        word2mpn w0(0); _SHOW(w0.n);
        const uint64_t u = _rand.full<uint64_t>();
        std::cerr << std::hex; _SHOW(u); std::cerr << std::dec;
        natural  n1(u);  _SHOW(n1);
        word2mpn w1(u);  _SHOW(w1.n);
        n0=n1; _SHOW(n0);
        n0=0;  _SHOW(n0);
        n0=u;  _SHOW(n0);
    }
}
YOCTO_UNIT_TEST_DONE()
