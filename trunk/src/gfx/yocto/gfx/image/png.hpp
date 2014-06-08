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
                                  image::put_rgba_proc   proc,
                                  const void            *args) const;
            
            virtual void     save(const string        &filename,
                                  const bitmap        &bmp,
                                  image::get_rgba_proc proc,
                                  const void          *args,
                                  const char          *options) const;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(png_format);
        };
    }
}

#endif

