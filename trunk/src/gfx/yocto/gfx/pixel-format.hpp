#ifndef YOCTO_GFX_PIXEL_FORMAT_INCLUDED
#define YOCTO_GFX_PIXEL_FORMAT_INCLUDED 1

#include "yocto/gfx/rgb.hpp"
#include "yocto/gfx/metrics.hpp"

namespace yocto
{
    namespace gfx
    {
        
        typedef rgba<uint8_t>  rgba_t;
        typedef rgba<uint32_t> mask_t;
        
        //! default RGBA format
        class pixel_format : public metrics
        {
        public:
            const mask_t   mask;
            const rgba_t   bits;
            const rgba_t   shift;
            const rgba_t   loss;
            const size_t   bpp;
            
            explicit pixel_format( uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask );
            virtual ~pixel_format() throw();
            pixel_format( const pixel_format &fmt ) throw();
            
            rgb_t   get_rgb(pixel_t C) const throw();
            rgba_t  get_rgba(pixel_t C) const throw();
            pixel_t map_rgb(const rgb_t &C) const throw();//!< assuming opaque
            pixel_t map_rgb(const rgb_t &C, uint8_t A) const throw();
            pixel_t map_rgba(const rgba_t &C) const throw();//!< assuming opaque
            
            
            
            friend std::ostream & operator<<( std::ostream &, const pixel_format &fmt );
            
        private:
            YOCTO_DISABLE_ASSIGN(pixel_format);
            
        };
        
        pixel_format RGB24();
        pixel_format RGBA32();
        pixel_format ARGB32();
        pixel_format RGBA16();
        pixel_format ARGB16();
        
    }
}
#endif
