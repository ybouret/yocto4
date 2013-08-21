#ifndef YOCTO_GFX_PIXEL_BUFFER_INCLUDED
#define YOCTO_GFX_PIXEL_BUFFER_INCLUDED 1

#include "yocto/gfx/metrics.hpp"
#include "yocto/counted.hpp"

namespace yocto
{
    namespace gfx
    {
        
        class pixbuf : public metrics, public counted
        {
        public:
            const size_t w;              //!< width
            const size_t h;              //!< height
            const size_t bytes_per_line; //!< w * bytes_per_pixel
            const size_t stride;         //!< may be larger than bytes_per_line
            
            //! allocate some pixels
            explicit pixbuf( size_t BytesPerPixel, size_t Width, size_t Height);
            virtual ~pixbuf() throw();
            
        protected:
            size_t bytes;
            void  *entry;
            
        public:
            const bool allocated;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(pixbuf);
        };
        
    }
}

#endif

