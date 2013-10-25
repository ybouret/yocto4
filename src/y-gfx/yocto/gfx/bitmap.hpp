#ifndef YOCTO_GFX_BITMAP_INCLUDED
#define YOCTO_GFX_BITMAP_INCLUDED 1

#include "yocto/gfx/types.hpp"
#include "yocto/counted.hpp"
#include "yocto/ptr/arc.hpp"

namespace yocto
{
    
    namespace gfx
    {
        
        class bitmap : public object, public counted
        {
        public:
            enum memory_type
            {
                in_memory,
                from_data,
                is_shared
            };
            
            typedef arc_ptr<bitmap> pointer;
            
            //! default destructor
            virtual ~bitmap() throw();
            
            //__________________________________________________________________
            //
            // data/metrics
            //__________________________________________________________________
            const memory_type type;      //!< memory
            const size_t      depth;     //!< bytes per pixel
            const size_t      w;         //!< width
            const size_t      h;         //!< height
            const size_t      pitch;     //!< w * depth
            const size_t      stride;    //!< >= scanline
            void             *entry;     //!< address of (0,0)
            const size_t      allocated; //!< if has memory
            
            //__________________________________________________________________
            //
            // API
            //__________________________________________________________________
            void ldz() throw(); //!< zero all bits
            
            //__________________________________________________________________
            //
            // constructors
            //__________________________________________________________________
            //! make an in_memory bitmap
            explicit bitmap(size_t Depth, size_t W, size_t H);
            
            //! make a shared bitmap
            explicit bitmap(const bitmap::pointer &bmp, unit_t x, unit_t y, size_t w, size_t h);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(bitmap);
            bitmap           *shared;    //!< in case of shared bitmap
            
        };
        
    }
    
}

#endif

