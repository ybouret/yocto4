#ifndef YOCTO_GFX_IMAGE_JPEG_INCLUDED
#define YOCTO_GFX_IMAGE_JPEG_INCLUDED 1

#include "yocto/gfx/image.hpp"

namespace yocto
{
    namespace gfx
    {
        class jpeg_format : public image::format
        {
        public:
            explicit jpeg_format();
            virtual ~jpeg_format() throw();
            
            virtual bool lossless() const throw();
            
            void load(const string &filename) const;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(jpeg_format);
        };
    }
}

#endif

