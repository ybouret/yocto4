#ifndef YOCTO_GFX_BITMAP_INCLUDED
#define YOCTO_GFX_BITMAP_INCLUDED 1

#include "yocto/gfx/region.hpp"
#include "yocto/counted.hpp"
#include "yocto/ptr/arc.hpp"

namespace yocto
{
    
    namespace gfx
    {
        
        class bitmap : public virtual object, public counted
        {
        public:
            enum memory_type
            {
                in_memory,
                from_data,
                is_shared
            };
            
            typedef arc_ptr<bitmap> pointer;
            typedef void * (*peek_proc)(void *,unit_t);
                      
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
            
            const peek_proc   peek;      //!< for address translation
            const size_t      allocated; //!< if has memory
            
            //__________________________________________________________________
            //
            // API
            //__________________________________________________________________
            void ldz() throw(); //!< zero all bits
            
            static bitmap * create(size_t Depth, size_t W, size_t H);
            static bitmap * link( const bitmap::pointer &bmp, const region *rect);
            
            void *       get_line(unit_t y) throw();
            const void * get_line(unit_t y) const throw();
            void *       hmove(void *,      unit_t x) throw();
            const void  *hmove(const void *,unit_t x) const throw();
            
            void *      get(unit_t x, unit_t y) throw();
            const void *get(unit_t x, unit_t y) const throw();
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(bitmap);
            bitmap           *shared;    //!< in case of shared bitmap
            
        protected:
            //__________________________________________________________________
            //
            // constructors
            //__________________________________________________________________
            //! make an in_memory bitmap
            explicit bitmap(size_t Depth, size_t W, size_t H);
            
            //! make a shared bitmap
            explicit bitmap(const bitmap::pointer &bmp, const region &rect );
            
        };
        
    }
    
}

#endif

