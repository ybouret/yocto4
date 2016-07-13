#ifndef YOCTO_GFX_TYPES_INCLUDED
#define YOCTO_GFX_TYPES_INCLUDED 1

#include "yocto/math/point2d.hpp"
#include "yocto/math/complex.hpp"
#include "yocto/parallel/patch.hpp"

namespace yocto
{
    namespace gfx
    {
        typedef point2d<unit_t>       vertex; //!< base class for operations
        typedef parallel::patch2D     patch;  //!< base class for patches
        typedef float                 real_t; //!< default floating point
        typedef math::complex<real_t> cplx_t; //!< complex for graphics...


#define YOCTO_GFX_R2GS 0.2126f
#define YOCTO_GFX_G2GS 0.7152f
#define YOCTO_GFX_B2GS 0.0722f

        struct gist
        {
            static const  float  unit_float[256]; //!< 0.0f..1.0f
            static const  float  unit_float_r[256]; //!< (0.0f..1.0f)*YOCTO_GFX_R2GS
            static const  float  unit_float_g[256]; //!< (0.0f..1.0f)*YOCTO_GFX_G2GS
            static const  float  unit_float_b[256]; //!< (0.0f..1.0f)*YOCTO_GFX_B2GS
            static inline float  greyscalef(const uint8_t R,const uint8_t G,const uint8_t B) throw()
            {
                return unit_float_r[R] + unit_float_g[R] + unit_float_b[B];
            }

            static const vertex delta[8]; //!< the 4+4 coordinates around a single point

        };
    }
}

#endif

