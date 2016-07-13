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


        struct gist
        {
            static const float unit_float[256];
        };
    }
}

#endif

