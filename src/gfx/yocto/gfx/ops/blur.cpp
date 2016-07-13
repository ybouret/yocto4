#include "yocto/gfx/ops/blur.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    using namespace math;

    namespace gfx
    {

        blur_info:: ~blur_info() throw()
        {
        }

        static inline
        unit_t __blur_width(const float sig)
        {
            const float exp_min = 1.0f/256;
            const float len_max = sqrtf( - 2.0f*sig*sig*logf(exp_min) );
            return max_of<unit_t>(1,unit_t(ceilf(len_max)));
        }

        blur_info:: blur_info(const float sig) :
        __width( __blur_width(sig) )
        {

        }

        blur:: ~blur() throw()
        {

        }


        blur:: blur(const float sig) :
        blur_info(sig),
        pixmap<float>( __width, __width ),
        top(__width-1), target(0), source(0)
        {
            const float den = 2.0f * sig * sig;
            for(unit_t j=0;j<=top;++j)
            {
                const float j2 = float(j*j);
                for(unit_t i=0;i<=top;++i)
                {
                    const float i2 = float(i*i);
                    (*this)[j][i]  = expf(-(i2+j2)/den );
                }
            }
        }
        
        
    }
}
