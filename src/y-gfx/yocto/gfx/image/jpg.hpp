#ifndef YOCTO_GFX_IMAGE_JPG_INCLUDED
#define YOCTO_GFX_IMAGE_JPG_INCLUDED 1

#include "yocto/gfx/image.hpp"

namespace yocto
{
    namespace gfx
    {
        
        class JPG : public image::format
        {
        public:
            explicit JPG();
            virtual ~JPG() throw();
            
            virtual void save(const string &filename,
                              const bitmap &bmp,
                              addr2rgba    &proc,
                              const char   *options) const;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(JPG);
        };
        
    }
}

#endif

