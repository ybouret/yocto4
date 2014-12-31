#ifndef YOCTO_GFX_IMAGE_INCLUDED
#define YOCTO_GFX_IMAGE_INCLUDED 1

#include "yocto/gfx/bitmap.hpp"
#include "yocto/gfx/rgb.hpp"

namespace yocto
{
    namespace gfx
    {
        
        class image
        {
        public:
            typedef void (*put_rgba_proc)(void *addr, const rgba_t &C, const void *args);
            typedef rgba_t (*get_rgba_proc)(const void *,const void *);
            
            
            class format : public counted_object
            {
            public:
                const string name;
                
                virtual ~format() throw();
                
                virtual bool     lossless() const throw() = 0;
                virtual bitmap  *load(const string         &filename,
                                      unit_t                depth,
                                      image::put_rgba_proc  proc,
                                      const void           *args) const = 0;
                
                virtual void     save(const string        &filename,
                                      const bitmap        &bmp,
                                      image::get_rgba_proc proc,
                                      const void          *args,
                                      const char          *options) const = 0;
                
                virtual const char **extension() const throw() = 0;
                
            protected:
                explicit format(const char *id);
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(format);
            };
            
        };
    }
}

#endif
