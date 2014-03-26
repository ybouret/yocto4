#include "yocto/utest/run.hpp"
#include "yocto/fovea/array3d.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/hashing/sha1.hpp"

using namespace yocto;
using namespace fovea;

YOCTO_UNIT_TEST_IMPL(array3d)
{
    hashing::sha1 H;
    for(size_t iter=1;iter<=10;++iter)
    {
        const unit_t lox = -unit_t(1 + alea_lt(100));
        const unit_t hix =  unit_t(1 + alea_lt(100));
        const unit_t loy = -unit_t(1 + alea_lt(100));
        const unit_t hiy =  unit_t(1 + alea_lt(100));
        const unit_t loz = -unit_t(1 + alea_lt(100));
        const unit_t hiz =  unit_t(1 + alea_lt(100));
        
        const coord3D lo(lox,loy,loz);
        const coord3D hi(hix,hiy,hiz);
        
        const layout3D L(lo,hi);
        
        array3D<ptrdiff_t> a3("a3",L);
        H.set();
        
        for(unit_t z=a3.lower.z;z<=a3.upper.z;++z)
        {
            for(unit_t y=a3.lower.y;y<=a3.upper.y;++y)
            {
                for(unit_t x=a3.lower.x;x<=a3.upper.x;++x)
                {
                    const ptrdiff_t r = alea_leq(100000);
                    H.run_type(r);
                    a3[z][y][x] = r;
                }
            }
        }
        const uint64_t k1 = H.key<uint64_t>();
        const uint64_t k2 = H.key<uint64_t>(a3.entry,a3.bytes);
        if(k1!=k2)
            throw exception("bad filling..");

        
    }
    
}
YOCTO_UNIT_TEST_DONE()
