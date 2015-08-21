#ifndef YOCTO_GFX_RGB_INCLUDED
#define YOCTO_GFX_RGB_INCLUDED 1


#include "yocto/gfx/types.hpp"
#include "yocto/bitwise.hpp"
#include <iostream>

namespace yocto
{
    namespace gfx
    {

        template <typename T>
        struct opaque
        {
            static const  T value;
            static T        scale(float x) throw();
        };

        template <typename T>
        class rgb
        {
        public:
            T r,g,b;

            inline rgb() throw() : r(0), g(0), b(0) {}
            inline rgb( T R, T G, T B ) throw() : r(R), g(G), b(B) {}
            inline rgb( const rgb &other ) throw() : r(other.r), g(other.g), b(other.b) {}
            inline rgb & operator=( const rgb &other ) throw()
            {
                r = other.r;
                g = other.g;
                b = other.b;
                return *this;
            }

            friend inline std::ostream & operator<<( std::ostream &os, const rgb<T> &C )
            {
                os << '[' << double(C.r) << ' ' << double(C.g) << ' ' << double(C.b) << ']';
                return os;
            }

            static inline rgb make_ramp(const float v) throw()
            {
                static const float vmin = 0.0f;
                static const float vmax = 1.0f;
                static const float dv   = vmax-vmin;
                float R=1,G=1,B=1;

                if (v <= vmin)
                {
                    R=0;
                    G=0;
                }
                else
                {
                    if (v >= vmax)
                    {
                        B=0;
                        G=0;
                    }
                    else
                    {
                        if (v < (vmin + 0.25f * dv))
                        {
                            R = 0;
                            G = 4.0f * (v - vmin) / dv;
                        } else if (v < (vmin + 0.5f * dv))
                        {
                            R = 0;
                            B = 1.0f + 4.0f * (vmin + 0.25f * dv - v) / dv;
                        } else if (v < (vmin + 0.75f * dv))
                        {
                            R = 4.0f * (v - vmin - 0.5f * dv) / dv;
                            B = 0;
                        } else {
                            G = 1.0f + 4.0f * (vmin + 0.75f * dv - v) / dv;
                            B = 0;
                        }
                    }
                }
                return rgb( opaque<T>::scale(R), opaque<T>::scale(G), opaque<T>::scale(B) );
            }

            
        };

        typedef rgb<uint8_t> rgb_t;


        template <typename T>
        class rgba
        {
        public:
            T r,g,b,a;
            inline  rgba() throw() : r(0), g(0), b(0), a( opaque<T>::value ) {}
            inline  rgba( T R, T G, T B ) throw() : r(R), g(G), b(B), a( opaque<T>::value ) {}
            inline  rgba( T R, T G, T B, T A ) throw() : r(R), g(G), b(B), a(A) {}

            inline ~rgba() throw() {}
            inline rgba( const rgba   &other) throw() : r(other.r), g(other.g), b(other.b), a(other.a) {}
            inline rgba( const rgb<T> &other) throw() : r(other.r), g(other.g), b(other.b), a(opaque<T>::value) {}

            inline rgba & operator=( const rgba &other ) throw()
            {
                r = other.r;
                g = other.g;
                b = other.b;
                a = other.a;
                return *this;
            }

            //! Keep the alpha channel !
            inline rgba & operator=( const rgb<T> &other ) throw()
            {
                r = other.r;
                g = other.g;
                b = other.b;
                return *this;
            }



            friend inline std::ostream & operator<<( std::ostream &os, const rgba<T> &C )
            {
                os << '[' << double(C.r) << ' ' << double(C.g) << ' ' << double(C.b) << ' ' << double(C.a) << ']';
                return os;
            }

            static inline rgba make_ramp(float x) throw() { return rgba( rgb<T>::make_ramp(x) ); }
        };

        typedef rgba<uint8_t> rgba_t;

        //______________________________________________________________________
        //
        //! converting rgb to a float value
        //______________________________________________________________________
        template <typename RGB_TYPE>
        inline float rgb2gsf(const RGB_TYPE &C) throw()
        {
            return greyscale<float>(C.r, C.g, C.b);
        }

        template <typename RGB_TYPE>
        inline float rgb2bwf(const RGB_TYPE &C) throw()
        {
            return black_and_white<float>(C.r, C.g, C.b);
        }


        //______________________________________________________________________
        //
        //! converting rgb to a byte value
        //______________________________________________________________________
        template <typename RGB_TYPE>
        inline uint8_t rgb2gs8(const RGB_TYPE &C) throw()
        {
            return greyscale<uint8_t>(C.r, C.g, C.b);
        }

        template <typename RGB_TYPE>
        inline uint8_t rgb2bw8(const RGB_TYPE &C) throw()
        {
            return black_and_white<uint8_t>(C.r, C.g, C.b);
        }


        //______________________________________________________________________
        //
        //! converting rgb to a word value
        //______________________________________________________________________
        template <typename RGB_TYPE>
        inline uint16_t rgb2gs16(const RGB_TYPE &C) throw()
        {
            return greyscale<uint16_t>(C.r, C.g, C.b);
        }

        template <typename RGB_TYPE>
        inline uint16_t rgb2bw16(const RGB_TYPE &C) throw()
        {
            return black_and_white<uint16_t>(C.r, C.g, C.b);
        }


        //______________________________________________________________________
        //
        //! color inversion prototype
        //______________________________________________________________________
        template <typename T>
        T invert_color(const T &) throw();

        template <>
        inline float invert_color<float>(const float &f) throw() { return 1.0f - f; }

        template <>
        inline double invert_color<double>(const double &f) throw() { return 1.0 - f; }

        template <>
        inline rgb_t invert_color<rgb_t>(const rgb_t &c ) throw()
        {
            return rgb_t(0xff-c.r,0xff-c.g,0xff-c.b);
        }

        template <>
        inline rgba_t invert_color<rgba_t>(const rgba_t &c ) throw()
        {
            return rgba_t(0xff-c.r,0xff-c.g,0xff-c.b,c.a);
        }

        //______________________________________________________________________
        //
        //! testing zero pixel
        //______________________________________________________________________

        template <typename T>
        bool is_zero_pixel(const T &) throw();

        template <>
        inline bool is_zero_pixel<uint8_t>(const uint8_t &u) throw()
        {
            return (u <= 0);
        }

        template <>
        inline bool is_zero_pixel<float>(const float &u) throw()
        {
            return fabsf(u) <= 0.0f;
        }

        template <>
        inline bool is_zero_pixel<double>(const double &u) throw()
        {
            return fabs(u) <= 0.0;
        }

        template <>
        inline bool is_zero_pixel<rgb_t>(const rgb_t &c) throw()
        {
            return (c.r<=0) && (c.g<=0) && (c.b<=0);
        }

        template <>
        inline bool is_zero_pixel<rgba_t>(const rgba_t &c) throw()
        {
            return (c.r<=0) && (c.g<=0) && (c.b<=0);
        }



    }
}

YOCTO_SUPPORT_C_STYLE_OPS(gfx::rgb<uint8_t>);
YOCTO_SUPPORT_C_STYLE_OPS(gfx::rgb<uint32_t>);
YOCTO_SUPPORT_C_STYLE_OPS(gfx::rgb<float>);
YOCTO_SUPPORT_C_STYLE_OPS(gfx::rgb<double>);
YOCTO_SUPPORT_C_STYLE_OPS(gfx::rgba<uint8_t>);
YOCTO_SUPPORT_C_STYLE_OPS(gfx::rgba<uint32_t>);
YOCTO_SUPPORT_C_STYLE_OPS(gfx::rgba<float>);
YOCTO_SUPPORT_C_STYLE_OPS(gfx::rgba<double>);


#endif
