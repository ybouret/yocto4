#include "yocto/graphics/rgb.hpp"
#include <cmath>
#include <cfloat>

namespace yocto
{
    namespace graphics
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

        template <>
        const double channel_info<double>::opaque = 1.0;

        template <>
        bool channel_info<double>::is_zero(const double c) throw()
        {
            return fabs(c)<=DBL_MIN;
        }



    }

}


namespace yocto
{
    namespace graphics
    {
        template <> bool is_zero_pixel<float>(const float &x) throw()
        {
            return channel_info<float>::is_zero(x);
        }

        template <> bool is_zero_pixel<uint8_t>(const uint8_t &x) throw()
        {
            return (x<=0);
        }

        template <> bool is_zero_pixel<size_t>(const size_t &x) throw()
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
    namespace graphics
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

namespace yocto
{
    namespace graphics
    {
        template <> float   to_float<float> (const float &x) throw()
        {
            return x;
        }

        template <> float   to_float<uint8_t> (const uint8_t &x) throw()
        {
            return gist::unit_float[x];
        }

        template <> float   to_float<RGB> (const RGB &x) throw()
        {
            return gist::greyscalef(x.r, x.g, x.b);
        }

        template <> float   to_float<RGBA> (const RGBA &x) throw()
        {
            return gist::greyscalef(x.r, x.g, x.b);
        }
    }
}


namespace yocto
{
    namespace graphics
    {
        template <> uint8_t invert_color<uint8_t>(const uint8_t &c) throw()
        {
            return 255-c;
        }

        template <> float invert_color<float>(const float &c) throw()
        {
            return 1.0f-c;
        }

        template <> RGB invert_color<RGB>(const RGB &c) throw()
        {
            return RGB(255-c.r,255-c.g,255-c.b);
        }

        template <> RGBA invert_color<RGBA>(const RGBA &c) throw()
        {
            return RGBA(255-c.r,255-c.g,255-c.b,c.a);
        }
        

    }
}


