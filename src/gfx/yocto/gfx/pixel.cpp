#include "yocto/gfx/pixel.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
    namespace gfx
    {
        ////////////////////////////////////////////////////////////////////////
        //
        // opaque
        //
        ////////////////////////////////////////////////////////////////////////
        template <>
        const uint8_t pixel<uint8_t>::opaque = 0xff;

        template <>
        const float pixel<float>::opaque = 1.0f;

        template <>
        const double pixel<double>::opaque = 1.0;


        ////////////////////////////////////////////////////////////////////////
        //
        // is_zero
        //
        ////////////////////////////////////////////////////////////////////////
        template <>
        bool pixel<uint8_t>::is_zero(const uint8_t x) throw()
        {
            return (x<=0);
        }

        template <>
        bool pixel<float>::is_zero(const float x) throw()
        {
            return fabsf(x) <= math::numeric<float>::minimum;
        }

        template <>
        bool pixel<double>::is_zero(const double x) throw()
        {
            return fabs(x) <= math::numeric<double>::minimum;
        }


        ////////////////////////////////////////////////////////////////////////
        //
        // project
        //
        ////////////////////////////////////////////////////////////////////////
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

        ////////////////////////////////////////////////////////////////////////
        //
        // to_float
        //
        ////////////////////////////////////////////////////////////////////////
        template <>
        float pixel<uint8_t>::to_float(const uint8_t x) throw()
        {
            return x;
        }

        template <>
        float pixel<float>::to_float(const float x) throw()
        {
            return x;
        }

        ////////////////////////////////////////////////////////////////////////
        //
        // blend
        //
        ////////////////////////////////////////////////////////////////////////
        template <>
        float pixel<float>::blend(const float bg, const float fg, const uint8_t alpha) throw()
        {
            const int a = alpha;
            return ((255-a)*fg + a*bg)/255.0f;
        }


        template <>
        uint8_t pixel<uint8_t>::blend(const uint8_t fg,
                                      const uint8_t bg,
                                      const uint8_t alpha) throw()
        {
            switch(alpha)
            {
                case 0:   return fg;
                case 255: return bg;
                default:
                    break;
            }
            const unsigned FG   = fg;
            const unsigned BG   = bg;
            const unsigned WBG  = alpha;
            const unsigned WFG  = 255-alpha;
            const unsigned C    = (FG*WFG + BG*WBG)/255;
            return uint8_t(C);
        }

        ////////////////////////////////////////////////////////////////////////
        //
        // invert
        //
        ////////////////////////////////////////////////////////////////////////
        template <>
        uint8_t pixel<uint8_t>::invert(const uint8_t x) throw()
        {
            return 0xff-x;
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


        ////////////////////////////////////////////////////////////////////////
        //
        // average
        //
        ////////////////////////////////////////////////////////////////////////
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

        template <>
        uint8_t pixel<uint8_t>:: average(const uint8_t lhs, const uint8_t rhs) throw()
        {
            const unsigned L = lhs;
            const unsigned R = rhs;
            return (L+R)>>1;
        }

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

        template <>
        float pixel<float>:: average(const float lhs, const float rhs) throw()
        {
            return 0.5f*(lhs+rhs);
        }

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

        template <>
        double pixel<double>:: average(const double lhs, const double rhs) throw()
        {
            return 0.5*(lhs+rhs);
        }

        

    }

}