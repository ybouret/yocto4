#include "yocto/gfx/ramp.hpp"
#include <cmath>

namespace yocto
{
    namespace gfx
    {

        ramp:: ~ramp() throw()
        {}

        ramp:: ramp( const rgb_t *user256rgb ) throw() :
        colors(user256rgb)
        {
            assert(colors);
        }

        rgb_t ramp:: operator[](size_t i) const throw()
        {
            return colors[i&0xff];
        }

        rgb_t ramp:: operator()(const double x) const throw()
        {
            if(x<=0)
            {
                return colors[0];
            }
            else
            {
                if(x>=1)
                {
                    return colors[255];
                }
                else
                {
                    return colors[ uint8_t(floor(255.0*x+0.5)) ];
                }
            }
        }


    }
}
