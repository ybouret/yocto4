#include "yocto/utest/run.hpp"
#include "yocto/mpa/natural.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ios/ocstream.hpp"
#include <iostream>

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(primality)
{
    const char *ntab[] =
    {
        "1373653",
        "25326001",
        "25000000000",
        "2152302898747"
    };
    vector<mpn> nn;
    for(size_t i=0;i<sizeof(ntab)/sizeof(ntab[0]);++i)
    {
        const mpn x = ntab[i];
        nn.push_back(x);
        std::cerr << x << " / " << x.bits() << std::endl;
    }
}
YOCTO_UNIT_TEST_DONE()
