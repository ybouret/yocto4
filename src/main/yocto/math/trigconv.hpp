#ifndef YOCTO_MATH_TRIGCONV_INCLUDED
#define YOCTO_MATH_TRIGCONV_INCLUDED 1

#include "yocto/math/types.hpp"

namespace yocto
{
    namespace math
    {

        inline float  Deg2Rad( const float  angle_deg ) throw() { return numeric<float>::pi  * angle_deg / 180.0f; }
        inline double Deg2Rad( const double angle_deg ) throw() { return numeric<double>::pi * angle_deg / 180.0;  }
    }

}

#endif