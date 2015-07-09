#ifndef YOCTO_GFX_BITMAP_INCLUDED
#define YOCTO_GFX_BITMAP_INCLUDED 1

#include "yocto/gfx/rectangle.hpp"
#include "yocto/counted-object.hpp"
#include "yocto/ptr/arc.hpp"

namespace yocto
{
    
    namespace gfx
    {
        
        //! memory handling for 1,2,3,4,8,16 bytes
        /**
         flexible bitmap: linear memory
         */
        class bitmap : public counted_object
        {
        public:
            //__________________________________________________________________
            //
            // Description
            //__________________________________________________________________

            typedef arc_ptr<bitmap> pointer;
            
            enum memory_type
            {
                memory_allocated, //!< in global memory
                memory_from_user, //!< from an array...
                memory_is_shared  //!< from another bitmap
            };
            
            typedef void * (*peek_proc)(const void *, unit_t dx);
            typedef void   (*swap_proc)(void *,void *);
            
            const unit_t      d;      //!< depth (in bytes)
            const unit_t      w;      //!< width
            const unit_t      h;      //!< height
            const unit_t      pitch;  //!< w*d
            const unit_t      stride; //!< stride >= pitch: jump one row
            void             *entry;  //!< the (0,0) coordinate for this bitmap
            const peek_proc   peek;
            const swap_proc   swap;
            const memory_type model;
            
            virtual ~bitmap() throw();
            
            
            //! memory_allocated bitmap
            explicit bitmap(unit_t depth, unit_t width, unit_t height);
            
            //! memory_allocated bitmap/copy
            explicit bitmap( const bitmap &bmp, const rectangle *rect);
            
            //! memory_is_shared bitmap
            explicit bitmap( const bitmap::pointer &bmp, const rectangle *rect = 0);
            
            //! memory_from_user
            explicit bitmap( unit_t depth, unit_t width, unit_t height, void *data, unit_t data_stride);
            
            
            //__________________________________________________________________
            //
            // Get addresses
            //__________________________________________________________________
            bool  owns(const rectangle &rect) const throw();
            void *get_line(unit_t y) const throw();      //!< \warning no check
            void *get(unit_t x, unit_t y) const throw(); //!< \warning no check

            void  safe_copy(void *, unit_t x, unit_t y, unit_t bytes) const throw(); //!< copy if within, 0 otherwise

            //__________________________________________________________________
            //
            // basic ops
            //__________________________________________________________________
            void  flip_horz() throw();
            void  flip_vert() throw();
            void  clear() throw();
            void  clear_borders() throw();
            
            static void check_depths( const char *lhs_name, const int lhs, const char *rhs_name, const int rhs);
            
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(bitmap);
            union
            {
                size_t  allocated;
                bitmap *shared;
            };
            
        };
        
    }
    
}

#endif
