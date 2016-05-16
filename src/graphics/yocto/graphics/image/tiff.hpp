#ifndef YOCTO_GFX_IMAGE_TIFF_INCLUDED
#define YOCTO_GFX_IMAGE_TIFF_INCLUDED 1

#include "yocto/graphics/image.hpp"

namespace yocto
{
    namespace graphics
    {

        class tiff_format : public image::format
        {
        public:
            explicit tiff_format();
            virtual ~tiff_format() throw();
            
            virtual bool     lossless() const throw();

            //! option is &indx, uint32_t
            virtual bitmap  *load(const string          &filename,
                                  unit_t                 depth,
                                  rgba2data             &proc,
                                  const void            *options) const;
            
            virtual void     save(const string        &filename,
                                  const bitmap        &bmp,
                                  data2rgba           &proc,
                                  const void          *options) const;


            uint32_t count_directories(const string &filename) const;

            //! load a bitmap with 0<=indx<count_directories
            bitmap *load_bitmap(const string          &filename,
                                unit_t                 depth,
                                rgba2data             &proc,
                                const uint32_t         indx) const;
            


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(tiff_format);
            virtual const char **extensions() const throw();
        };

    }

}

#endif


