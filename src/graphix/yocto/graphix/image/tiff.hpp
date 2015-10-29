#ifndef YOCTO_GFX_IMAGE_TIFF_INCLUDED
#define YOCTO_GFX_IMAGE_TIFF_INCLUDED 1

#include "yocto/graphix/image.hpp"
#include "yocto/tiff++/interface.hpp"
#include "yocto/rtld/plugin.hpp"

namespace yocto
{
    namespace graphix
    {

        class tiff_format : public image::format
        {
        public:
            explicit tiff_format();
            virtual ~tiff_format() throw();
            
            virtual bool     lossless() const throw();

            virtual bitmap  *load(const string          &filename,
                                  unit_t                 depth,
                                  image::put_rgba_proc   proc,
                                  void                  *args,
                                  const void            *options) const;
            
            virtual void     save(const string        &filename,
                                  const bitmap        &bmp,
                                  image::get_rgba_proc proc,
                                  void                *args,
                                  const void          *options) const;


            uint32_t count_directories(const string &filename) const;

            bitmap *load_bitmap(const string          &filename,
                                unit_t                 depth,
                                image::put_rgba_proc   proc,
                                void                  *args,
                                const uint32_t         indx) const;

        private:
            plugin<I_TIFF> api;
            YOCTO_DISABLE_COPY_AND_ASSIGN(tiff_format);
            virtual const char **extensions() const throw();
        };

    }

}

#endif


