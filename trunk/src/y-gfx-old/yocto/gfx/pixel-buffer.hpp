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
            typedef void * (*peek_proc)(void*,unit_t);
            
            const size_t    w;              //!< width
            const size_t    h;              //!< height
            const size_t    bytes_per_line; //!< w * bytes_per_pixel
            const size_t    stride;         //!< may be larger than bytes_per_line
            const peek_proc peek;           //!< peek and address on a line
            
            //! allocate some pixels
            explicit pixbuf( size_t BytesPerPixel, size_t Width, size_t Height);
            
            //! clean up
            virtual ~pixbuf() throw();
            
            class pointer
            {
            public:
                //! p must be valid
                pointer( pixbuf *p ) throw();
                ~pointer() throw();
                pointer(const pointer &other ) throw();
                
                pixbuf *operator->() throw();
                const pixbuf * operator->() const throw();
                
            private:
                YOCTO_DISABLE_ASSIGN(pointer);
                pixbuf *host;
            };
            
            size_t      bytes() const throw();
            void       *entry( unit_t x, unit_t y ) throw();
            
        private:
            size_t bytes_;
            void  *entry_;
            
        public:
            const bool allocated;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(pixbuf);
        };
        
        typedef pixbuf::pointer pixel_buffer;

    }
    
}

#endif

