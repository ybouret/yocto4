#ifndef YOCTO_MATH_TRIGCONV_INCLUDED
#define YOCTO_MATH_TRIGCONV_INCLUDED 1

#include "yocto/math/types.hpp"

namespace yocto
{
    namespace math
    {

        inline float  Deg2Rad( const float  angle_deg ) throw() { return numeric<float>::pi  * angle_deg / 180.0f; }
        inline double Deg2Rad( const double angle_deg ) throw() { return numeric<double>::pi * angle_deg / 180.0;  }
        inline float  Rad2Deg( const float  angle_rad ) throw() { return 180.0f * angle_rad / numeric<float>::pi;  }
        inline double Rad2Deg( const double angle_rad ) throw() { return 180.0  * angle_rad / numeric<double>::pi; }

        inline double CosDeg( const double angle_deg ) throw() { return Cos( Deg2Rad(angle_deg) ); }
        inline double SinDeg( const double angle_deg ) throw() { return Sin( Deg2Rad(angle_deg) ); }
        inline float  CosDeg( const float  angle_deg ) throw() { return Cos( Deg2Rad(angle_deg) ); }
        inline float  SinDeg( const float  angle_deg ) throw() { return Sin( Deg2Rad(angle_deg) ); }
    }

}

#endif