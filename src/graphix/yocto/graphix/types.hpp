#ifndef YOCTO_GRAPHIX_TYPES_INCLUDED
#define YOCTO_GRAPHIX_TYPES_INCLUDED 1

#include "yocto/parallel/splitter.hpp"
#include <cmath>

namespace yocto
{
    namespace graphix
    {
        typedef point2d<unit_t>   vertex; //!< base class for operations
        typedef parallel::patch2D patch;  //!< base class for patches

        struct gist
        {
            static const  float   unit_float[256];
            static inline uint8_t float2byte(const float x) throw() { return uint8_t(floorf(255.0f*x+0.5f)); }
            static inline float   greyscale(const float R,const float G,const float B) throw()
            {
                const float r = 0.2126f*R;
                const float g = 0.7152f*G;
                const float b = 0.0722f*B;
                const float Y = r + g + b;
                return Y;
            }
        };
    }
}

#endif
