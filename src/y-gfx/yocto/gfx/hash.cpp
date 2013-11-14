#include "yocto/gfx/hash.hpp"

namespace yocto
{
    namespace gfx
    {
        
        void hash_bitmap( const bitmap &bmp, hashing::function &H  ) throw()
        {
            const uint8_t *p = static_cast<const uint8_t *>(bmp.entry);
            for(unit_t j=0;j<bmp.h;++j)
            {
                H.run(p, bmp.pitch);
                p += bmp.stride;
            }
        }
        
    }

}
