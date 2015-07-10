#ifndef YOCTO_GFX_PIXMAPS_INCLUDED
#define YOCTO_GFX_PIXMAPS_INCLUDED 1

#include "yocto/gfx/pixmap.hpp"

namespace yocto
{
    namespace gfx
    {
        //! stack of 1..size pixmaps, same dimensions
        template <typename T>
        class pixmaps
        {
        public:
            const size_t size;
            explicit pixmaps(size_t n,const unit_t W, const unit_t H) :
            size(n>0?n:1),
            npx(size),
            pxm( memory::kind<memory::global>::acquire_as< pixmap<T> >(npx) ),
            built(0)
            {
                try
                {
                }
                catch(...)
                {
                    kill();
                    throw;
                }
            }

            virtual ~pixmaps() throw()
            {
                kill();
            }

        private:
            size_t     npx;
            pixmap<T> *pxm;
            size_t     built; //!< for building

            YOCTO_DISABLE_COPY_AND_ASSIGN(pixmaps);
            inline void kill() throw()
            {
                while(built>0)
                {
                    --built;
                }
                memory::kind<memory::global>::release_as< pixmap<T> >(pxm, npx);
            }
        };
    }
}

#endif

