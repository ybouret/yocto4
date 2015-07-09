#ifndef YOCTO_GFX_BITMAP_INCLUDED
#define YOCTO_GFX_BITMAP_INCLUDED 1

#include "yocto/gfx/rectangle.hpp"
#include "yocto/counted-object.hpp"
#include "yocto/ptr/arc.hpp"

namespace yocto
{
    namespace gfx
    {


        //! handling linear memory
        /**
         for items of size 1,2,3,4,8,16
         */
        class bitmap : public counted_object
        {
        public:
            typedef arc_ptr<bitmap> pointer;

            enum memory_type
            {
                memory_allocated, //!< in memory
                memory_is_shared, //!< pointing to another bitmap
                memory_from_user  //!< using local/static data
            };

            const unit_t      d;      //!< depth in bytes: 1,2,3,4,8,16
            const unit_t      w;      //!< width
            const unit_t      h;      //!< height
            const unit_t      pitch;  //!< w*d, size of a scan line
            const unit_t      stride; //!< bytes to move one line up/down, stride>=pitch
            void             *entry;  //!< address of the (0,0) item
            const memory_type model;

            explicit bitmap(const unit_t D, const unit_t W, const unit_t H);

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

