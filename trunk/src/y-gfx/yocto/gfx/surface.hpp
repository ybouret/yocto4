#ifndef YOCTO_GFX_SURFACE_INCLUDED
#define YOCTO_GFX_SURFACE_INCLUDED 1

#include "yocto/gfx/bitmap.hpp"
#include "yocto/gfx/format.hpp"

namespace yocto
{
    namespace gfx
    {
        class surface : public format, public bitmap
        {
        public:
            static surface *create( const format fmt, unit_t W, unit_t H);
            virtual ~surface() throw();
            
        private:
            explicit surface( const format &fmt, unit_t W, unit_t H);

            YOCTO_DISABLE_COPY_AND_ASSIGN(surface);
            
        };
    }
    
}

#endif

