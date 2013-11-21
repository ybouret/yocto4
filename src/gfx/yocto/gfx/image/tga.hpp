#ifndef YOCTO_GFX_IMAGE_TGA_INCLUDED
#define YOCTO_GFX_IMAGE_TGA_INCLUDED 1

#include "yocto/gfx/image.hpp"

namespace yocto
{
    namespace gfx
    {
        
        class TGA : public image::format
        {
        public:
            explicit TGA();
            virtual ~TGA() throw();
            
            virtual void save(const string &filename, 
								 const bitmap &bmp,
								 addr2rgba    &proc,
								 const char   *options) const;
            
            virtual surface *load(const string      &filename,
                                  const pixel_format fmt) const;
            
            virtual bool lossless() const throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(TGA);
        };
        
    }
}

#endif

