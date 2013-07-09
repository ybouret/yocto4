#include "yocto/gfx/color-format.hpp"
#include "yocto/exception.hpp"
#include "yocto/code/gsort.hpp"
#include <iostream>

namespace yocto
{
    
    namespace gfx
    {
        
        color_format:: ~color_format() throw() {}
        
        namespace
        {
            struct mask_id
            {
                uint32_t    mask;
                const char *name;
            };
        }
        static inline
        void __check_masks(const mask_id &lhs, const mask_id &rhs )
        {
            assert(lhs.name);
            assert(rhs.name);
            if(lhs.mask&rhs.mask) throw exception("overlapping %s and %s masks", lhs.name, rhs.name);
        }
        
        void color_format:: check_masks() const
        {
            if(bytes_per_pixel<=0||bytes_per_pixel>4)
                throw exception("unhandled bytes_per_pixel=%u", unsigned(bytes_per_pixel));
            
            const mask_id reg[4] =
            {
                { mask.r, "red"   },
                { mask.g, "green" },
                { mask.b, "blue"  },
                { mask.a, "alpha" }
            };
            
            for(size_t i=0;i<3;++i)
            {
                const mask_id &lhs = reg[i];
                for(size_t j=i+1;j<4;++j)
                {
                    __check_masks(lhs, reg[j]);
                }
            }
        }
        
        void color_format:: get_metrics()
        {
            check_masks();
            color_bits::fmt(mask.r, bits.r, shift.r, loss.r, "r");
            color_bits::fmt(mask.g, bits.g, shift.g, loss.g, "g");
            color_bits::fmt(mask.b, bits.b, shift.b, loss.b, "b");
            color_bits::fmt(mask.a, bits.a, shift.a, loss.a, "a");
            std::cerr << "R=" << color_bits::binary(mask.r) << ": " << int(bits.r) << "+" << int(shift.r) << std::endl;
            std::cerr << "G=" << color_bits::binary(mask.g) << ": " << int(bits.g) << "+" << int(shift.g) << std::endl;
            std::cerr << "B=" << color_bits::binary(mask.b) << ": " << int(bits.b) << "+" << int(shift.b) << std::endl;
            std::cerr << "A=" << color_bits::binary(mask.a) << ": " << int(bits.a) << "+" << int(shift.a) << std::endl;

            
            uint8_t msb[4] = {
                bits.r + shift.r,
                bits.g + shift.g,
                bits.b + shift.b,
                bits.a + shift.a
            };
            c_sort(msb,4);
            std::cerr << "highest bits@" << int(msb[3]) << std::endl;
            (uint8_t &)bits_per_pixel = msb[3];
            if(bits_per_pixel>bytes_per_pixel*8)
                throw exception("Invalid bits_per_pixel=%u for bytes_per_pixel=%u", unsigned(bits_per_pixel), unsigned(bytes_per_pixel));
        }
        
        color_format:: color_format(uint8_t BPP,
                                    uint32_t rmask, uint32_t gmask, uint32_t bmask, uint32_t amask) :
        mask(rmask,gmask,bmask,amask),
        bits(),
        shift(),
        loss(),
        bytes_per_pixel(BPP),
        bits_per_pixel(0)
        {
            get_metrics();
        }
        
    }
    
}
