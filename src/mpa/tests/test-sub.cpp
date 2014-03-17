#include "yocto/utest/run.hpp"
#include "yocto/mpa/natural.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(sub)
{
    
    vector<mpn> u;
    for(size_t i=200;i>0;--i)
    {
        const mpn x = _rand.full<uint64_t>();
        u.push_back(x);
    }
    quicksort(u, mpn::compare );
    for(size_t i=u.size();i>1;--i)
    {
        const mpn lhs = u[i];
        const mpn rhs = u[i-1];
        const mpn x   = lhs  - rhs;
        const mpn y   = x + rhs;
        if( lhs != y )
            throw exception("sub/add failure");
    }
    
    mpn j = _rand.full<uint16_t>();
    while(j>0)
    {
        
    }

}
YOCTO_UNIT_TEST_DONE()
