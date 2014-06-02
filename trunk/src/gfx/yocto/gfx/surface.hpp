#ifndef YOCTO_GFX_SURFACE_INCLUDED
#define YOCTO_GFX_SURFACE_INCLUDED 1

#include "yocto/gfx/pixel-format.hpp"
#include "yocto/gfx/bitmap.hpp"


namespace yocto
{
    namespace gfx
    {
        
        class surface : public bitmap
        {
        public:
            const pixel_format format;
            
            virtual ~surface() throw();
            explicit surface( const pixel_format fmt, unit_t width, unit_t height);
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(surface);
        };
        
    }
}

#endif
