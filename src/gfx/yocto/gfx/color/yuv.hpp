#ifndef YOCTO_GFX_COLOR_YUV_INCLUDED
#define YOCTO_GFX_COLOR_YUV_INCLUDED 1

#include "yocto/gfx/color/rgb.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace gfx
    {

        class YUV
        {
        public:
            typedef float type;
            float y,u,v;
            inline  YUV(const float Y=0,const float U=0,const float V=0) throw() : y(Y), u(U), v(V) {}
            inline ~YUV() throw() {}
            inline  YUV(const YUV &other) throw() : y(other.y), u(other.u), v(other.v) {}
            inline  YUV & operator=( const YUV &other ) throw()
            {
                y=other.y; u=other.u; v=other.v;
                return *this;
            }

            static inline
            RGB toRGB(const YUV &C) throw()
            {
                const float rf = C.y                + 1.140f*C.v;
                const float gf = C.y - 0.314f * C.u - 0.581f*C.v;
                const float bf = C.y - 2.028f * C.u;
                return RGB(gist::float2byte(clamp<float>(0,rf,1)),
                           gist::float2byte(clamp<float>(0,gf,1)),
                           gist::float2byte(clamp<float>(0,bf,1))
                           );
            }

            static inline
            YUV fromRGB(const RGB &C ) throw()
            {
                const float rf = gist::unit_float[C.r];
                const float gf = gist::unit_float[C.g];
                const float bf = gist::unit_float[C.b];
                const float Y  =  0.299f*rf+0.587f*gf+0.114f*bf;
                const float U  = -0.147f*rf-0.289f*rf+0.437f*bf;
                const float V  =  0.615f*rf-0.515f*gf-0.100f*bf;
                return YUV(Y,U,V);
            }

        };
    }
}

#endif
