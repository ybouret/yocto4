#include "yocto/gfx/pixel.hpp"

namespace yocto
{
    namespace gfx
    {

        template <>
        const uint8_t pixel<uint8_t>::opaque = 0xff;

        template <>
        const float pixel<float>::opaque = 1.0f;

        template <>
        const double pixel<double>::opaque = 1.0;


        // project

        template <>
        uint8_t pixel<uint8_t>::project(const uint8_t x) throw()
        {
            return x;
        }

        template <>
        uint8_t pixel<float>::project(const float x) throw()
        {
            return gist::float2byte(x);
        }

        // invert
        template <>
        uint8_t pixel<uint8_t>::invert(const uint8_t x) throw()
        {
            return 255-x;
        }

        template <>
        float pixel<float>::invert(const float x) throw()
        {
            return 1.0f-x;
        }

        template <>
        double pixel<double>::invert(const double x) throw()
        {
            return 1.0-x;
        }


        // average
        template <>
        uint8_t pixel<uint8_t>:: average(const array<uint8_t> &ra) throw()
        {
            const size_t n = ra.size();
            if(n>0)
            {
                float ans = 0;
                for(size_t i=n;i>0;--i)
                {
                    ans += float(ra[i]);
                }
                return uint8_t(ans/n);
            }
            else
            {
                return 0;
            }
        }

        // average
        template <>
        float pixel<float>:: average(const array<float> &ra) throw()
        {
            const size_t n = ra.size();
            if(n>0)
            {
                float ans = 0;
                for(size_t i=n;i>0;--i)
                {
                    ans += ra[i];
                }
                return ans/n;
            }
            else
            {
                return 0;
            }
        }

        // average
        template <>
        double pixel<double>:: average(const array<double> &ra) throw()
        {
            const size_t n = ra.size();
            if(n>0)
            {
                double ans = 0;
                for(size_t i=n;i>0;--i)
                {
                    ans += ra[i];
                }
                return ans/n;
            }
            else
            {
                return 0;
            }
        }


    }

}