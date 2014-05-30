#include "yocto/gfx/rgb.hpp"

namespace yocto
{
    namespace gfx
    {
        YOCTO_GFX_RGB_IMPL(rgb_mask_t, uint32_t, 0xffffffff)
        YOCTO_GFX_RGB_IMPL(rgb_t,      uint8_t,        0xff)

        unsigned rgb_t:: distance_sq( const rgb_t &lhs, const rgb_t &rhs ) throw()
        {
            const int dr = int(lhs.r) - int(rhs.r);
            const int dg = int(lhs.g) - int(rhs.g);
            const int db = int(lhs.b) - int(rhs.b);
            return dr*dr + dg*dg + db*db;
        }
        
        uint8_t rgb_t:: grey() const throw()
        {
            return conv::greyscale(r, g, b);
        }

    }
}
