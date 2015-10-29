#ifndef YOCTO_GFX_IMAGE_PNG_INCLUDED
#define YOCTO_GFX_IMAGE_PNG_INCLUDED 1

#include "yocto/graphix/image.hpp"

namespace yocto
{
    namespace graphix
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
                                  void                  *args,
                                  const void            *options) const;
            
            virtual void     save(const string        &filename,
                                  const bitmap        &bmp,
                                  image::get_rgba_proc proc,
                                  void                  *args,
                                  const void          *options) const;
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(png_format);
            virtual const char **extensions() const throw();
        };
    }
}

#endif

