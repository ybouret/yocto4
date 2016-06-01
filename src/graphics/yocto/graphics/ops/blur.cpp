#include "yocto/graphics/ops/blur.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    using namespace math;

    namespace graphics
    {

        blur:: ~blur() throw()
        {
            
        }

        static inline
        unit_t __blur_width(const float sig)
        {
            const float exp_min = numeric<float>::epsilon;
            const float len_max = (exp_min >= 1.0f) ? 0.0f : sqrtf( - 2.0*sig*sig* logf(exp_min) );
            return max_of<unit_t>(1,ceilf(len_max));
        }

        blur:: blur(const float sig) :
        pixmap<float>( __blur_width(sig), __blur_width(sig) ),
        top(width.x-1)
        {
            const float den = 2.0f * sig * sig;
            for(unit_t j=0;j<=top;++j)
            {
                const float j2 = j*j;
                for(unit_t i=0;i<=top;++i)
                {
                    const float i2 = i*i;
                    (*this)[j][i]  = expf(-(i2*j2)/den);
                }
            }
        }
        

    }
}
