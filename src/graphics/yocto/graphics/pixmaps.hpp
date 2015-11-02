#ifndef YOCTO_GRAPHICS_PIXMAP_INCLUDED
#define YOCTO_GRAPHICS_PIXMAP_INCLUDED 1

#include "yocto/graphix/pixmap.hpp"
#include "yocto/sequence/slots.hpp"

namespace yocto
{
    namespace graphix
    {

        template <typename T>
        class pixmaps : public slots_of< pixmap<T> >, public patch
        {
        public:
            const unit_t w;
            const unit_t h;
            
            inline explicit pixmaps(const size_t n,
                                    const unit_t W,
                                    const unit_t H) :
            slots_of< pixmap<T> >(n),
            patch( vertex(0,0), vertex( bitmap::check_w(W)-1, bitmap::check_h(H)-1 ) ),
            w( width.x ),
            h( width.y )
            {
                for(size_t k=0;k<n;++k)
                {
                    this->template append<unit_t,unit_t>(w,h);
                }
            }

            inline virtual ~pixmaps() throw()
            {

            }


        private:

            YOCTO_DISABLE_COPY_AND_ASSIGN(pixmaps);
        };

    }
}

#endif

