#ifndef YOCTO_GFX_FORMAT_INCLUDED
#define YOCTO_GFX_FORMAT_INCLUDED 1

#include "yocto/gfx/rgb.hpp"

namespace yocto
{
    namespace gfx
    {
        
        typedef rgba<uint8_t>  rgba_t;
        typedef rgba<uint32_t> mask_t;
        
        
        class format
        {
        public:
            typedef pixel_t (*get_proc)(const void *);
            typedef void    (*put_proc)(void *,pixel_t);
           
            const mask_t   mask;
            const size_t   depth;
            const get_proc get_pixel;
            const put_proc put_pixel;
            const rgba_t   bits;
            const rgba_t   shift;
            const rgba_t   loss;
            const size_t   bpp;
            
            format( uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask );
            ~format() throw();
            format( const format &fmt ) throw();
            
           
            
            
        private:
            YOCTO_DISABLE_ASSIGN(format);
            
        };
        
    }
}
#endif
