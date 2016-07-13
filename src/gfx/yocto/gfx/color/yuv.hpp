#ifndef YOCTO_GFX_COLOR_YUV_INCLUDED
#define YOCTO_GFX_COLOR_YUV_INCLUDED 1

#include "yocto/gfx/color/rgb.hpp"

namespace yocto
{
    namespace gfx
    {

        class YUV
        {
        public:
            float y,u,v;
            inline  YUV(const float Y=0.0f,const float U=0.0f,const float V=0.0f) throw() : y(Y), u(U), v(V) {}
            inline ~YUV() throw() {}
            inline  YUV(const YUV &other) throw() : y(other.y), u(other.u), v(other.v) {}
            inline  YUV & operator=( const YUV &other ) throw()
            {
                y=other.y; u=other.u; v=other.v;
                return *this;
            }
        };
    }
}

#endif
