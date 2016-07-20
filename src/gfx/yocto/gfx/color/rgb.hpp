#ifndef YOCTO_GFX_COLOR_RGB_INCLUDED
#define YOCTO_GFX_COLOR_RGB_INCLUDED 1

#include "yocto/gfx/pixel.hpp"

#if defined(RGB)
#undef RGB
#endif

#if defined(RGBA)
#undef RGBA
#endif

namespace yocto
{
    namespace gfx
    {

#define YOCTO_GFX_COMPARE_OP(TYPE,OP) \
inline friend bool operator OP ( const TYPE &lhs, const TYPE &rhs ) throw() { return TYPE::compare(lhs,rhs) OP 0; }

#define YOCTO_GFX_COMPARE(TYPE) \
YOCTO_GFX_COMPARE_OP(TYPE,==)\
YOCTO_GFX_COMPARE_OP(TYPE,!=)\
YOCTO_GFX_COMPARE_OP(TYPE,<)\
YOCTO_GFX_COMPARE_OP(TYPE,>)\
YOCTO_GFX_COMPARE_OP(TYPE,<=)\
YOCTO_GFX_COMPARE_OP(TYPE,>=)

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

            inline friend std::ostream & operator<<( std::ostream &os, const rgb<T> &C )
            {
                os << '(' << double(C.r) << ',' << double(C.g) << ',' << double(C.b) << ')';
                return os;
            }

            inline real_t intensity() const throw()
            {
                return YOCTO_GFX_R2GS * real_t(r) + YOCTO_GFX_G2GS * real_t(g) + YOCTO_GFX_B2GS * real_t(b);
            }

            static inline int compare(const rgb &lhs, const rgb &rhs) throw()
            {
                const real_t L = lhs.intensity();
                const real_t R = rhs.intensity();
                return (L<R) ? -1 : (  (R<L) ? 1 : 0);
            }

            YOCTO_GFX_COMPARE(rgb<T>)

            static inline float to_float( const rgb &C ) throw() { return gist::greyscalef(C.r,C.g,C.b); }
        };

        template <typename T>
        class rgba
        {
        public:
            typedef T type;

            T r,g,b,a;
            inline rgba() throw() : r(0), g(0), b(0), a( pixel<T>::opaque ) {}
            inline rgba(const T R,const T G, const T B, const T A) throw() : r(R), g(G), b(B), a(A) {}
            inline rgba(const T R,const T G, const T B) throw() : r(R), g(G), b(B), a( pixel<T>::opaque ) {}
            inline rgba(const rgba &other) throw() : r(other.r),g(other.g),b(other.b), a(other.a) {}
            inline rgba & operator=( const rgba &other ) throw()
            {
                r = other.r; g=other.g; b=other.b; a=other.a;
                return *this;
            }

            inline rgba(const rgb<T> &other) throw() : r(other.r),g(other.g),b(other.b), a( pixel<T>::opaque ) {}

            inline rgb<T>       & _rgb() throw()       { return *(rgb<T>       *)this; }
            inline const rgb<T> & _rgb() const throw() { return *(const rgb<T> *)this; }


            inline friend std::ostream & operator<<( std::ostream &os, const rgba<T> &C )
            {
                os << '(' << double(C.r) << ',' << double(C.g) << ',' << double(C.b) << ',' << double(C.a) << ')';
                return os;
            }

            static inline int compare(const rgba &lhs, const rgba &rhs) throw()
            {
                return rgb<T>::compare( lhs._rgb(), rhs._rgb() );
            }

            static inline float to_float( const rgba &C ) throw() { return gist::greyscalef(C.r,C.g,C.b); }

            YOCTO_GFX_COMPARE(rgba<T>)

        };

        typedef rgb<uint8_t>  RGB;
        typedef rgba<uint8_t> RGBA;

        //______________________________________________________________________
        //
        // RGB specs
        //______________________________________________________________________
        template <> inline
        uint8_t pixel<RGB>::project( const RGB C ) throw()
        {
            return gist::greyscale1(C.r,C.g,C.b);
        }

        template <> inline
        RGB pixel<RGB>::invert(const RGB C) throw()
        {
            return RGB(255-C.r,255-C.g,255-C.b);
        }

        template <> inline
        RGB pixel<RGB>:: average(const array<RGB> &ra) throw()
        {
            const size_t n = ra.size();
            if(n>0)
            {
                rgb<float> ans(0,0,0);
                for(size_t i=n;i>0;--i)
                {
                    const RGB &C = ra[i];
                    ans.r += float(C.r);
                    ans.g += float(C.g);
                    ans.b += float(C.b);
                }
                return RGB( uint8_t(ans.r/n), uint8_t(ans.g/n), uint8_t(ans.b/n) );
            }
            else
            {
                return RGB(0,0,0);
            }
        }

        template <> inline
        bool pixel<RGB>::is_zero(const RGB C) throw()
        {
            return (C.r<=0) && (C.g<=0) && (C.b<=0);
        }

        template <> inline
        RGB pixel<RGB>::blend(const RGB fg,const RGB bg,const uint8_t alpha) throw()
        {
            switch(alpha)
            {
                case 0:   return fg;
                case 255: return bg;
                default: break;
            }
            const unsigned WBG = alpha;
            const unsigned WFG = 256-WBG;
            const unsigned R   = unsigned(fg.r)*WFG + unsigned(bg.r)*WBG;
            const unsigned G   = unsigned(fg.g)*WFG + unsigned(bg.g)*WBG;
            const unsigned B   = unsigned(fg.b)*WFG + unsigned(bg.b)*WBG;
            return RGB(R>>8,G>>8,B>>8);
        }

        template <> inline
        float pixel<RGB>::to_float(const RGB C) throw()
        {
            return RGB::to_float(C);
        }

        //______________________________________________________________________
        //
        // RGBA specs
        //______________________________________________________________________
        template <> inline
        uint8_t pixel<RGBA>::project( const RGBA C ) throw()
        {
            return gist::greyscale1(C.r,C.g,C.b);
        }

        template <> inline
        RGBA pixel<RGBA>::invert(const RGBA C) throw()
        {
            return RGBA(255-C.r,255-C.g,255-C.b);
        }

        template <> inline
        RGBA pixel<RGBA>:: average(const array<RGBA> &ra) throw()
        {
            const size_t n = ra.size();
            if(n>0)
            {
                rgba<float> ans(0,0,0);
                for(size_t i=n;i>0;--i)
                {
                    const RGBA &C = ra[i];
                    ans.r += float(C.r);
                    ans.g += float(C.g);
                    ans.b += float(C.b);
                }
                return RGBA( uint8_t(ans.r/n), uint8_t(ans.g/n), uint8_t(ans.b/n) );
            }
            else
            {
                return RGBA(0,0,0);
            }
        }

        template <> inline
        float pixel<RGBA>::to_float(const RGBA C) throw()
        {
            return RGBA::to_float(C);
        }

    }
}

#endif
