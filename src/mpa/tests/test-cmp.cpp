#include "yocto/utest/run.hpp"
#include "yocto/mpa/natural.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(cmp)
{
    
    vector<mpn> u;
    for(size_t i=20;i>0;--i)
    {
        const mpn x = alea_leq(1000000);
        u.push_back(x);
    }
    std::cerr << "u=" << u << std::endl;
    quicksort(u, mpn::cmp );
    std::cerr << "u=" << u << std::endl;
}
YOCTO_UNIT_TEST_DONE()
