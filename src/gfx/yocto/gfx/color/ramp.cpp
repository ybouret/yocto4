#include "yocto/gfx/color/ramp.hpp"

namespace yocto
{
    namespace gfx
    {
        ramp:: ~ramp() throw()
        {
        }

        ramp:: ramp(const RGBA *user_codes) throw() :
        vmin(0),
        vmax(0),
        codes(user_codes)
        {
            assert(codes!=NULL);
        }

        RGBA ramp::compute(const float v) const throw()
        {
            if(v<=vmin)
            {
                return codes[0];
            }
            else
            {
                if(v>=vmax)
                {
                    return codes[255];
                }
                else
                {
                    const float alpha = (v-vmin)/(vmax-vmin);
                    return codes[ gist::float2byte(alpha) ];
                }
            }
        }

    }
}
