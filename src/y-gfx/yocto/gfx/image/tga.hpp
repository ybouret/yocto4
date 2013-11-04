#ifndef YOCTO_GFX_IMAGE_TGA_INCLUDED
#define YOCTO_GFX_IMAGE_TGA_INCLUDED 1

#include "yocto/gfx/image.hpp"

namespace yocto
{
    namespace gfx
    {
        
        class TGA : public image::format
        {
        public:
            explicit TGA();
            virtual ~TGA() throw();
            
            YOCTO_GFX_IMAGE_FORMAT_API();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(TGA);
        };
        
    }
}

#endif

