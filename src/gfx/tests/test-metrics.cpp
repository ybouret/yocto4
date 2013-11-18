#include "yocto/utest/run.hpp"
#include "yocto/gfx/metrics.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(metrics)
{
    uint8_t bits = 0;
    uint8_t shft = 0;
    uint8_t loss = 0;
    
    uint32_t w = 0;
    for(int nbits=0;nbits<=8;++nbits)
    {
        for(int s=0; s <= 32-nbits; ++s)
        {
            const uint32_t mask = w << s;
            std::cerr << "mask=" << gfx::conv::to_binary(mask) << std::endl;
            gfx::metrics::fmt(mask, bits, shft, loss, "generic");
            std::cerr << "bits=" << int(bits) << " | ";
            std::cerr << "shft=" << int(shft) << " | ";
            std::cerr << "loss=" << int(loss) << std::endl;
            std::cerr << std::endl;
            if( int(bits) != nbits )
                throw exception("invalid bits analysis");
            
            if( mask && int(shft) != s )
                throw exception("invalid shift analysis");
        }
        w <<= 1;
        w  |= 1;
    }
    
}
YOCTO_UNIT_TEST_DONE()
