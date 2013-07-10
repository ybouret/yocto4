#ifndef YOCTO_GFX_SURFACE_INCLUDED
#define YOCTO_GFX_SURFACE_INCLUDED 1

#include "yocto/gfx/color-format.hpp"
#include "yocto/gfx/pixel-buffer.hpp"

namespace yocto
{
    namespace gfx
    {
        
        class surface : public object
        {
        public:
            virtual ~surface() throw();
            
            //! make a new memory surface
            explicit surface( const color_format &fmt, size_t width, size_t height);
            
            const color_format format;
            const pixel_buffer pixels;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(surface);
        };
    }
}

#endif


