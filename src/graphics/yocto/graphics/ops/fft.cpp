#include "yocto/graphics/ops/fft.hpp"
#include "yocto/math/fft/fftn.hpp"

namespace yocto
{
    namespace graphics
    {
        void fft:: forward( pixmapz &img ) throw()
        {
            assert(img.model==bitmap::memory_is_global);
            float       *addr  = & (img[0][0].re);
            const size_t nn[4] = { 0, img.w, img.h, 0 };
            math::FourierN<float>::Forward(addr-1, nn, 2);
        }
    }
}
