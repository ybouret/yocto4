#ifndef YOCTO_GRAPHICS_OPS_BLOBS_INCLUDED
#define YOCTO_GRAPHICS_OPS_BLOBS_INCLUDED 1

#include "yocto/graphics/pixmap.hpp"
#include "yocto/graphics/rgb.hpp"
#include "yocto/stock/stack.hpp"

namespace yocto
{
    namespace graphics
    {

        class blobs : public pixmap<size_t>
        {
        public:
            explicit blobs(const unit_t W, const unit_t H);
            virtual ~blobs() throw();

            size_t        current;
            stack<vertex> vstk;

            template <typename T>
            void build(const pixmap<T> &src,
                       const size_t     link = 4 )
            {
                pixmap<size_t> &self = *this;
                assert(w==src.w);
                assert(h==src.h);
                assert(link==4||link==8);
                ldz();
                vstk.free();
                for(unit_t j=0;j<h;++j)
                {
                    // scan over rows
                    row &                          B_j = self[j];
                    const typename pixmap<T>::row &S_j = src[j];

                    for(unit_t i=0;i<w;++i)
                    {
                        // study where we are
                        size_t &B = B_j[i];
                        if(B_j[i]<=0 && !is_zero_pixel(S_j[i]) )
                        {

                        }
                    }
                }

            }

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(blobs);
        };

    }

}

#endif

