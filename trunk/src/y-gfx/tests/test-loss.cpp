#include "yocto/utest/run.hpp"
#include "yocto/gfx/color-format.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(loss)
{
    for(unsigned loss=0;loss<=8;++loss)
    {
        const unsigned bits = 8-loss;
        const unsigned xmul = (unsigned(1) << bits) - 1;
        std::cerr << "loss= " << loss << std::endl;
        std::cerr << "bits= " << bits << std::endl;
        std::cerr << "xmul= " << xmul << std::endl;
        
        for(unsigned i=0;i<256;++i)
        {
            const unsigned u = (i*xmul)/255;
            const unsigned v = i>>loss;
            if(u!=v)
                std::cerr << i << " => " << u << "," << v << std::endl;
        }
        
        std::cerr << std::endl;
    }
}
YOCTO_UNIT_TEST_DONE()
