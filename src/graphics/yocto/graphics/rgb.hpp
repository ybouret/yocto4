#ifndef YOCTO_GRAPHIX_RGB_INCLUDED
#define YOCTO_GRAPHIX_RGB_INCLUDED 1

#include "yocto/graphics/types.hpp"

#if defined(RGB)
#undef RGB
#endif

namespace yocto
{
    namespace graphics
    {

        template <typename T>
        struct channel_info
        {
            static const  T opaque;
            static bool     is_zero(const T) throw();
            static inline T scale(const double x) throw()
            {
                static const double amplitude( opaque );
                const T ans(amplitude*x);
                //std::cerr << x << "->" << double(ans) << std::endl;
                return ans;
            }
        };

        template <typename T>
        class rgb
        {
        public:
            typedef T type;
            T r,g,b;
            inline rgb() throw() : r(0), g(0), b(0) {}
            inline rgb(const T R,const T G, const T B) throw() : r(R), g(G), b(B) {}
            inline rgb(const rgb &other) throw() : r(other.r),g(other.g),b(other.b) {}
            inline rgb & operator=( const rgb &other ) throw()
            {
                r = other.r; g=other.g; b=other.b;
                return *this;
            }
        };



        template <typename T>
        class rgba
        {
        public:
            typedef T type;

            T r,g,b,a;
            inline rgba() throw() : r(0), g(0), b(0), a( channel_info<T>::opaque ) {}
            inline rgba(const T R,const T G, const T B, const T A) throw() : r(R), g(G), b(B), a(A) {}
            inline rgba(const T R,const T G, const T B) throw() : r(R), g(G), b(B), a( channel_info<T>::opaque ) {}
            inline rgba(const rgba &other) throw() : r(other.r),g(other.g),b(other.b), a(other.a) {}
            inline rgba & operator=( const rgba &other ) throw()
            {
                r = other.r; g=other.g; b=other.b; a=other.a;
                return *this;
            }

            inline rgba(const rgb<T> &other) throw() : r(other.r),g(other.g),b(other.b), a( channel_info<T>::opaque ) {}

        };


        typedef rgb<uint8_t>  RGB;
        typedef rgba<uint8_t> RGBA;

        //______________________________________________________________________
        //
        // abstract operations
        //______________________________________________________________________
        template <typename T> bool    is_zero_pixel(const T&) throw();
        template <typename T> uint8_t project(const T&) throw();
        template <typename T> float   to_float(const T&) throw();
        template <typename T> T       invert_color(const T&) throw();

        template <typename RGB_TYPE>
        inline RGB_TYPE default_ramp( double v, const double vmin=0, const double vmax=0)
        {

            double r=1,g=1,b=1;
            //std::cerr << v << "[" << vmin << ";" << vmax << "]" << std::endl;
            if(vmax>vmin)
            {
                if (v < vmin)
                    v = vmin;
                if (v > vmax)
                    v = vmax;
                const double dv = vmax - vmin;

                if (v < (vmin + 0.25 * dv)) {
                    r = 0;
                    g = 4 * (v - vmin) / dv;
                } else if (v < (vmin + 0.5 * dv)) {
                    r = 0;
                    b = 1 + 4 * (vmin + 0.25 * dv - v) / dv;
                } else if (v < (vmin + 0.75 * dv)) {
                    r = 4 * (v - vmin - 0.5 * dv) / dv;
                    b = 0;
                } else {
                    g = 1 + 4 * (vmin + 0.75 * dv - v) / dv;
                    b = 0;
                }
            }

            return RGB_TYPE(channel_info< typename RGB_TYPE::type>::scale(r),
                            channel_info< typename RGB_TYPE::type>::scale(g),
                            channel_info< typename RGB_TYPE::type>::scale(b));
        }
        
    }
    
}

YOCTO_SUPPORT_C_STYLE_OPS(graphics::rgb<uint8_t>);
YOCTO_SUPPORT_C_STYLE_OPS(graphics::rgb<float>);
YOCTO_SUPPORT_C_STYLE_OPS(graphics::rgba<uint8_t>);
YOCTO_SUPPORT_C_STYLE_OPS(graphics::rgba<float>);



#endif
