#ifndef YOCTO_GFX_PIXEL_BUFFER_INCLUDED
#define YOCTO_GFX_PIXEL_BUFFER_INCLUDED 1

#include "yocto/gfx/types.hpp"
#include "yocto/shared-ptr.hpp"
#include "yocto/memory/buffer.hpp"


namespace yocto
{
    namespace gfx
    {
        
        class pixbuf : public memory::rw_buffer
        {
        public:
            typedef void * (*peek_proc)(void *addr, unit_t x, unit_t y, const size_t *param);
            
            //! allocated memory
            explicit pixbuf( size_t BPP, size_t width, size_t height);
            
            //! user's memory
            explicit pixbuf( size_t BPP, size_t width, size_t height, void *user_data);
            
            virtual ~pixbuf() throw();
            virtual size_t length() const throw();
            
            
            void       *get(unit_t x, unit_t y) throw();
            const void *get(unit_t x, unit_t y) const throw();
            
            const size_t bytes_per_pixel;
            const size_t w;
            const size_t stride;
            const size_t h;
            const size_t bytes;  private:
            size_t       items;  public:
            void        *data;   private:
            peek_proc    peek;   public:
            const bool   allocated;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(pixbuf);
            virtual const void *get_address() const throw();
        };
        
        typedef shared_ptr<pixbuf> pixel_buffer;
        
    }
}

#endif

