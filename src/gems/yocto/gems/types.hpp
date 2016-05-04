#ifndef YOCTO_GEMS_TYPES_INCLUDED
#define YOCTO_GEMS_TYPES_INCLUDED 1

#include "yocto/memory/global.hpp"
#include "yocto/math/point3d.hpp"
#include <cmath>

namespace yocto
{
 
    namespace gems
    {


        typedef uint32_t                  word_t;
        typedef memory::global::allocator allocator;
        typedef point3d<unit_t>           coord_t;
        
        inline double ANINT(double x) throw() { return floor(x+0.5);   }
        inline float  ANINT(float  x) throw() { return floorf(x+0.5f); }
    }
    
}

#endif
