#ifndef YOCTO_GFX_IMAGE_PNG_INCLUDED
#define YOCTO_GFX_IMAGE_PNG_INCLUDED 1

#include "yocto/gfx/image.hpp"

namespace yocto
{
    namespace gfx
    {
        class png_format : public image::format
        {
        public:
            explicit png_format();
            virtual ~png_format() throw();
            
            virtual bool     lossless() const throw();
            virtual bitmap  *load(const string          &filename,
                                  unit_t                 depth,
                                  rgba2data             &proc,
                                  const void            *options) const;
            
            virtual void     save(const string        &filename,
                                  const bitmap        &bmp,
                                  data2rgba           &proc,
                                  const void          *options) const;
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(png_format);
            virtual const char **extensions() const throw();
        };
    }
}

#endif

