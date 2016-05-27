#ifndef YOCTO_GRAPHICS_OPS_FFT_INCLUDED
#define YOCTO_GRAPHICS_OPS_FFT_INCLUDED 1

#include "yocto/graphics/rawpix.hpp"
#include <cstring>

namespace yocto
{
    namespace graphics
    {

        class fourier
        {
        public:
            typedef math::real_of<cplx_t>::type real_t;

            //! forward transform
            /**
             \param pz must be a memory allocated, power of two dimensions
             */
            static void forward( pixmapz &pz ) throw();

            //! forward transform
            /**
             \param pz must be a memory allocated, power of two dimensions
             */
            static void reverse( pixmapz &pz ) throw();

            template <typename T> static inline
            void forward( pixmapz &pz, const pixmap<T> &pt ) throw()
            {
                assert(pt.w<=pz.w);
                assert(pt.h<=pz.h);
                const unit_t h = pt.h;
                const unit_t w = pt.w;
                const unit_t H = pz.h;
                const unit_t W = pz.w;
                const size_t part_bytes = (W-w)*sizeof(cplx_t);
                const size_t full_bytes = W*sizeof(cplx_t);
                for(unit_t j=0;j<h;++j)
                {
                    const typename pixmap<T>::row &rt = pt[j];
                    pixmapz::row                  &rz = pz[j];
                    for(unit_t i=0;i<w;++i)
                    {
                        rz[i] = real_t(rt[i]);
                    }
                    memset( &rz[w],0,part_bytes);
                }
                for(unit_t j=h;j<H;++j)
                {
                    memset(&pz[j][0],0,full_bytes);
                }
                forward(pz);
            }

        };

    }
}

#endif
