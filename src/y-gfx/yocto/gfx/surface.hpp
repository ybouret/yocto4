#ifndef YOCTO_GFX_SURFACE_INCLUDED
#define YOCTO_GFX_SURFACE_INCLUDED 1

#include "yocto/gfx/color-format.hpp"
#include "yocto/gfx/pixel-buffer.hpp"

namespace yocto
{
    namespace gfx
    {
        class surface
        {
        public:
            const color_format format;
            pixel_buffer       pixels;
            const size_t       w;
            const size_t       h;
            
            virtual ~surface() throw();
            explicit surface( const color_format fmt, size_t width, size_t height);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(surface);
        };
    }
}

#endif

