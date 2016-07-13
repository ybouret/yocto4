#ifndef YOCTO_GFX_IMAGE_IO_INCLUDED
#define YOCTO_GFX_IMAGE_IO_INCLUDED 1

#include "yocto/gfx/rawpix.hpp"
#include "yocto/gfx/color/rgba2data.hpp"
#include "yocto/gfx/color/data2rgba.hpp"

namespace yocto
{
    namespace gfx
    {
        class imageIO
        {
        public:
            virtual ~imageIO() throw();


            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual bitmap  *load(const string         &filename,
                                  unit_t                depth,
                                  rgba2data            &proc,
                                  const void           *options) const = 0;

            virtual void     save(const string        &filename,
                                  const bitmap        &bmp,
                                  data2rgba           &proc,
                                  const void          *options) const = 0;
            
            //__________________________________________________________________
            //
            // non  virtual LOAD interface
            //__________________________________________________________________
            bitmap *load4(const string &filename, const void *options) const;
            bitmap *load3(const string &filename, const void *options) const;
            bitmap *load1(const string &filename, const void *options) const;
            bitmap *loadf(const string &filename, const void *options) const;

            //______________________________________________________________
            //
            // Non virtual SAVE interface
            //______________________________________________________________
            void save(const string &filename, const pixmap4 &bmp, const char *options) const; //!< color, possible alpha
            void save(const string &filename, const pixmap3 &bmp, const char *options) const; //!< color
            void save(const string &filename, const pixmap1 &bmp, const char *options) const; //!< greyscale
            void save(const string &filename, const pixmapf &bmp, const char *options) const; //!< greyscale

            
        protected:
            explicit imageIO() throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(imageIO);
        };
    }
}

#endif

