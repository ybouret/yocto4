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
            YOCTO_DISABLE_COPY_AND_ASSIGN(jpeg_format);
        };
    }
}

#endif

