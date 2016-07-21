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

        template <typename RGB_TYPE>
        inline int compare_colors(const RGB_TYPE &lhs, const RGB_TYPE &rhs ) throw()
        {
            const float L = gist::greyscalef(lhs.r,lhs.g,lhs.b);
            const float R = gist::greyscalef(rhs.r,rhs.g,rhs.b);
            return (L<R) ? -1 : ( (R<L) ? 1 : 0 );
        }

        template <typename RGB_TYPE>
        inline RGB_TYPE average_colors(const RGB_TYPE &lhs, const RGB_TYPE &rhs ) throw()
        {
            const uint8_t r = uint8_t(0.5f*( float(lhs.r) + float(rhs.r) ));
            const uint8_t g = uint8_t(0.5f*( float(lhs.g) + float(rhs.g) ));
            const uint8_t b = uint8_t(0.5f*( float(lhs.b) + float(rhs.b) ));
            return RGB_TYPE(r,g,b);
        }

        template <typename RGB_TYPE>
        inline RGB_TYPE average_colors(const array<RGB_TYPE> &a) throw()
        {
            float r = 0;
            float g = 0;
            float b = 0;
            const size_t n = a.size();
            if(n>0)
            {
                for(size_t i=n;i>0;--i)
                {
                    const RGB_TYPE &C = a[i];
                    r += float(C.r);
                    g += float(C.g);
                    b += float(C.b);
                }
                r = floorf(r/n+0.5f);
                g = floorf(g/n+0.5f);
                b = floorf(b/n+0.5f);
            }
            return RGB_TYPE(uint8_t(r),uint8_t(g),uint8_t(b));
        }


        //______________________________________________________________________
        //
        // abstract operations
        //______________________________________________________________________
        template <typename T> bool    is_zero_pixel(const T&) throw();
        template <typename T> uint8_t project(const T&) throw();
        template <typename T> float   to_float(const T&) throw();
        template <typename T> T       invert_color(const T&) throw();
        
        //______________________________________________________________________
        //
        // making a default ramp
        //______________________________________________________________________
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
