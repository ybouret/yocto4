#include "yocto/mpl/natural.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;

#define _SHOW(X) std::cerr << #X " = " << X << std::endl

YOCTO_UNIT_TEST_IMPL(mpn)
{
    mpn n0; _SHOW(n0);
    mpn n1 = 0x12;     _SHOW(n1);
    mpn n2 = 0x0234;   _SHOW(n2);
    mpn n3 = 0x123456; _SHOW(n3);

}
YOCTO_UNIT_TEST_DONE()
