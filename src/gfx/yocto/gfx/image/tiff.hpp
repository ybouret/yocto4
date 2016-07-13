#ifndef YOCTO_GFX_IMAGE_TIFF_INCLUDED
#define YOCTO_GFX_IMAGE_TIFF_INCLUDED 1

#include "yocto/gfx/image.hpp"
#include "yocto/gfx/image/tiff++.hpp"

namespace yocto
{
    namespace gfx
    {

        class tiff_format : public image::format
        {
        public:
            explicit tiff_format();
            virtual ~tiff_format() throw();
            
            virtual bool     lossless() const throw();
            
            
            //! option input_options...
            virtual bitmap  *load(const string          &filename,
                                  unit_t                 depth,
                                  rgba2data             &proc,
                                  const void            *options) const;
            
            virtual void     save(const string        &filename,
                                  const bitmap        &bmp,
                                  data2rgba           &proc,
                                  const void          *options) const;


            size_t count_directories(const string &filename) const;

            //! load a bitmap with 0<=indx<count_directories
            bitmap *load_bitmap(const string          &filename,
                                unit_t                 depth,
                                rgba2data             &proc,
                                const size_t           indx) const;
            


        private:
            mutable I_TIFF::Raster raster;
            YOCTO_DISABLE_COPY_AND_ASSIGN(tiff_format);
            virtual const char **extensions() const throw();
        };

    }

}

#endif


