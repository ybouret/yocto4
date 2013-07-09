#include "yocto/gfx/types.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace gfx
    {
        void color_bits:: fmt(const uint32_t mask,
                              const uint8_t &bits_,
                              const uint8_t &shift_,
                              const uint8_t &loss_,
                              const char *which)
        {
            assert(which);
            uint8_t &bits  = (uint8_t &)bits_;
            uint8_t &shift = (uint8_t &)shift_;
            uint8_t &loss  = (uint8_t &)loss_;
            
            if(mask<=0)
            {
                bits  = 0;
                shift = 0;
                loss  = 8;
            }
            else
            {
                uint32_t pattern = 0;
                for(bits=1;bits<=8;++bits)
                {
                    pattern <<= 0x01;
                    pattern  |= 0x01;
                    const uint8_t top = 32-bits;
                    for(shift=0;shift<=top;++shift)
                    {
                        if( (pattern<<shift) == mask )
                        {
                            loss = 8-bits;
                            return;
                        }
                    }
                }
                throw exception("color_bits:fmt(invalid mask %s=0x%08x)",which,unsigned(mask));
            }
        }
        
        string color_bits::binary(const uint32_t c)
        {
            string ans(8,as_capacity);
            for(int shift=31;shift>=0;--shift)
            {
                if( 0 != ( (0x1<<shift) & c) )
                    ans += '1';
                else
                    ans += '0';
            }
            return ans;
        }
        
    }
}
