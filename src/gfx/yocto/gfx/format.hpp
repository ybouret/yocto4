#ifndef YOCTO_GFX_FORMAT_INCLUDED
#define YOCTO_GFX_FORMAT_INCLUDED 1

#include "yocto/gfx/rgb.hpp"
#include "yocto/gfx/metrics.hpp"

namespace yocto
{
    namespace gfx
    {
        
        typedef rgba<uint8_t>  rgba_t;
        typedef rgba<uint32_t> mask_t;
        
        
        class format : public metrics
        {
        public:
            const mask_t   mask;
            const rgba_t   bits;
            const rgba_t   shift;
            const rgba_t   loss;
            const size_t   bpp;
            
            format( uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask );
            ~format() throw();
            format( const format &fmt ) throw();
            
            rgb_t   get_rgb(pixel_t C) const throw();
            rgba_t  get_rgba(pixel_t C) const throw();
            pixel_t map_rgb(const rgb_t &C) const throw();//!< assuming opaque
            pixel_t map_rgb(const rgb_t &C, uint8_t A) const throw();
            pixel_t map_rgba(const rgba_t &C) const throw();//!< assuming opaque

        private:
            YOCTO_DISABLE_ASSIGN(format);
            
        };
        
    }
}
#endif
