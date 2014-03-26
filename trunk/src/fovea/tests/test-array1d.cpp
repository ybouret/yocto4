#include "yocto/utest/run.hpp"
#include "yocto/fovea/array1d.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/hashing/sha1.hpp"

using namespace yocto;
using namespace fovea;

YOCTO_UNIT_TEST_IMPL(array1d)
{
    hashing::sha1 H;
    
    for(size_t iter=1;iter<=10;++iter)
    {
        const unit_t lo = -unit_t(1 + alea_lt(100));
        const unit_t hi =  unit_t(1 + alea_lt(100));

        array1D<int> a1( "a1", layout1D(lo,hi) );
        std::cerr << a1 << std::endl;
        
        H.set();
        for(unit_t i=a1.lower;i<=a1.upper;++i)
        {
            const int r = alea<uint32_t>();
            H.run_type(r);
            a1[i] = r;
        }
        const uint64_t key1 = H.key<uint64_t>();
        const uint64_t key2 = H.key<uint64_t>( a1.entry, a1.bytes );
        if(key1!=key2)
            throw exception("bad filling...");
    }
}
YOCTO_UNIT_TEST_DONE()

