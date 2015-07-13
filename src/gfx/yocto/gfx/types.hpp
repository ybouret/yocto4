#ifndef YOCTO_GFX_TYPES_INCLUDED
#define YOCTO_GFX_TYPES_INCLUDED 1

#include "yocto/os.hpp"
#include <cmath>

namespace yocto
{
    namespace gfx
    {

        //! default pixel representation
        typedef uint32_t  pixel_t;

        template <typename T> uint8_t to_byte(const T &) throw();

        template <>
        inline uint8_t to_byte(const uint8_t &u) throw() { return u; }

        template <>
        inline uint8_t to_byte(const float   &f) throw() { return uint8_t( floorf(f*255.0f + 0.5f) ); }

        template <>
        inline uint8_t to_byte(const double  &d) throw() { return uint8_t( floor(d*255.0 + 0.5) ); }

        extern const float unit_float[256];

        template <typename T>
        T greyscale(const uint8_t R, const uint8_t G, const uint8_t B) throw();

        template <>
        inline float greyscale<float>(const uint8_t R,
                                      const uint8_t G,
                                      const uint8_t B) throw()
        {
            const float r = 0.2126f*unit_float[R] ;
            const float g = 0.7152f*unit_float[G];
            const float b = 0.0722f*unit_float[B];
            const float Y = r + g + b;
            return Y;
        }

        template <>
        inline uint8_t greyscale<uint8_t>(const uint8_t R,
                                          const uint8_t G,
                                          const uint8_t B) throw()
        {
            return to_byte<float>( greyscale<float>(R,G,B) );
        }

        template <>
        inline double greyscale<double>(const uint8_t R,
                                        const uint8_t G,
                                        const uint8_t B) throw()
        {
            return double( greyscale<float>(R,G,B) );
        }


        template <typename T>
        T black_and_white(const uint8_t R, const uint8_t G, const uint8_t B) throw();


        template <>
        inline float black_and_white<float>(const uint8_t R,
                                            const uint8_t G,
                                            const uint8_t B) throw()
        {
            return (unit_float[R]+unit_float[G]+unit_float[B])/3.0;
        }
        
        template <>
        inline uint8_t black_and_white<uint8_t>(const uint8_t R,
                                                const uint8_t G,
                                                const uint8_t B) throw()
        {
            return uint8_t( (unsigned(R)+unsigned(G)+unsigned(B))/3 );
        }

    }
    
}

#endif

