#include "yocto/utest/run.hpp"
#include "yocto/fame/array2d.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/hashing/sha1.hpp"

using namespace yocto;
using namespace fame;

YOCTO_UNIT_TEST_IMPL(array2d)
{
    hashing::sha1 H;
    for(size_t iter=1;iter<=10;++iter)
    {
        const unit_t lox = -unit_t(1 + alea_lt(100));
        const unit_t hix =  unit_t(1 + alea_lt(100));
        const unit_t loy = -unit_t(1 + alea_lt(100));
        const unit_t hiy =  unit_t(1 + alea_lt(100));
        array2D<double> a2( "a2", layout2D(coord2D(lox,loy),coord2D(hix,hiy) ));
        
        H.set();
        for(unit_t y=a2.lower.y;y<=a2.upper.y;++y)
        {
            for(unit_t x=a2.lower.x;x<=a2.upper.x;++x)
            {
                const double r = alea<double>();
                H.run_type(r);
                a2[y][x] = r;
            }
        }
        const uint64_t k1 = H.key<uint64_t>();
        const uint64_t k2 = H.key<uint64_t>(a2.entry,a2.bytes);
        if(k1!=k2)
            throw exception("bad filling..");
    }
}
YOCTO_UNIT_TEST_DONE()

