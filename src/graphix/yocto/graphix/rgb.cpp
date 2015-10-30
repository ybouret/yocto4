#include "yocto/graphix/rgb.hpp"
#include <cmath>
#include <cfloat>

namespace yocto
{
    namespace graphix
    {

        template <>
        const uint8_t channel_info<uint8_t>::opaque = 0xff;

        template <>
        bool channel_info<uint8_t>::is_zero(const uint8_t c) throw()
        {
            return (c<=0);
        }

        template <>
        const float channel_info<float>::opaque = 1.0f;

        template <>
        bool channel_info<float>::is_zero(const float c) throw()
        {
            return fabsf(c)<=FLT_MIN;
        }



    }

}


namespace yocto
{
    namespace graphix
    {
        template <> bool is_zero_pixel<float>(const float &x) throw()
        {
            return channel_info<float>::is_zero(x);
        }

        template <> bool is_zero_pixel<uint8_t>(const uint8_t &x) throw()
        {
            return (x<=0);
        }
        
        template <> bool is_zero_pixel<RGB>(const RGB &c) throw()
        {
            return (c.r<=0) && (c.g<=0) && (c.b<=0);
        }

        template <> bool is_zero_pixel<RGBA>(const RGBA &c) throw()
        {
            return (c.r<=0) && (c.g<=0) && (c.b<=0);
        }
    }
    
}

namespace yocto
{
    namespace graphix
    {
        template <> uint8_t project<float>(const float &x) throw()
        {
            return gist::float2byte(x);
        }

        template <> uint8_t project<uint8_t>(const uint8_t &x) throw()
        {
            return x;
        }

        template <> uint8_t project<RGB>(const RGB &c) throw()
        {
            return gist::greyscale1(c.r, c.g, c.b);
        }


        template <> uint8_t project<RGBA>(const RGBA &c) throw()
        {
            return gist::greyscale1(c.r, c.g, c.b);
        }



    }

}


