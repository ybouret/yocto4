#include "yocto/gfx/color-format.hpp"
#include "yocto/exception.hpp"
#include "yocto/code/gsort.hpp"
#include <iostream>

namespace yocto
{
    
    namespace gfx
    {
        
        color_format:: ~color_format() throw() {}
        
        namespace
        {
            struct mask_id
            {
                uint32_t    mask;
                const char *name;
            };
        }
        static inline
        void __check_masks(const mask_id &lhs, const mask_id &rhs )
        {
            assert(lhs.name);
            assert(rhs.name);
            if(lhs.mask&rhs.mask) throw exception("overlapping %s and %s masks", lhs.name, rhs.name);
        }
        
        void color_format:: check_masks() const
        {
            if(bytes_per_pixel<=0||bytes_per_pixel>4)
                throw exception("unhandled bytes_per_pixel=%u", unsigned(bytes_per_pixel));
            
            const mask_id reg[4] =
            {
                { mask.r, "red"   },
                { mask.g, "green" },
                { mask.b, "blue"  },
                { mask.a, "alpha" }
            };
            
            for(size_t i=0;i<3;++i)
            {
                const mask_id &lhs = reg[i];
                for(size_t j=i+1;j<4;++j)
                {
                    __check_masks(lhs, reg[j]);
                }
            }
        }
        
        static inline
        uint32_t get_pixel32( const void *addr ) throw()
        {
            assert(addr);
            return *(uint32_t *)addr;
        }
        
        
        static inline
        void set_pixel32( uint32_t c, void *addr ) throw()
        {
            assert(addr);
            *(uint32_t *)addr = c;
        }

        
        static inline
        uint32_t get_pixel16( const void *addr ) throw()
        {
            assert(addr);
            return *(uint16_t *)addr;
        }
        
        
        static inline
        void set_pixel16( uint32_t c, void *addr ) throw()
        {
            assert(addr);
            *(uint16_t *)addr = c;
        }
        
        static inline
        uint32_t get_pixel8( const void *addr ) throw()
        {
            assert(addr);
            return *(uint8_t *)addr;
        }
        
        
        static inline
        void set_pixel8( uint32_t c, void *addr ) throw()
        {
            assert(addr);
            *(uint8_t *)addr = c;
        }


        static inline
        uint32_t get_pixel24( const void *addr ) throw()
        {
            assert(addr);
            const uint8_t *p = (const uint8_t *)addr;
            const uint32_t R = p[0];
            const uint32_t G = p[1];
            const uint32_t B = p[2];
            return (R<<16) | (G<<8) | B;
        }
        
        
        static inline
        void set_pixel24( uint32_t c, void *addr ) throw()
        {
            assert(addr);
            const uint32_t R = (c>>16) & 0xff;
            const uint32_t G = (c>>8 ) & 0xff;
            const uint32_t B = (c    ) & 0xff;
            uint8_t *p = (uint8_t*)addr;
            p[0] = uint8_t(R);
            p[1] = uint8_t(G);
            p[2] = uint8_t(B);
        }

        
        
        void color_format:: get_metrics()
        {
            //------------------------------------------------------------------
            // check masks are not overlapping
            //------------------------------------------------------------------
            check_masks();
            
            //------------------------------------------------------------------
            // compute each channel
            //------------------------------------------------------------------
            color_bits::fmt(mask.r, bits.r, shift.r, loss.r, "r");
            color_bits::fmt(mask.g, bits.g, shift.g, loss.g, "g");
            color_bits::fmt(mask.b, bits.b, shift.b, loss.b, "b");
            color_bits::fmt(mask.a, bits.a, shift.a, loss.a, "a");
            std::cerr << "R=" << color_bits::binary(mask.r) << ": " << int(bits.r) << "+" << int(shift.r) << std::endl;
            std::cerr << "G=" << color_bits::binary(mask.g) << ": " << int(bits.g) << "+" << int(shift.g) << std::endl;
            std::cerr << "B=" << color_bits::binary(mask.b) << ": " << int(bits.b) << "+" << int(shift.b) << std::endl;
            std::cerr << "A=" << color_bits::binary(mask.a) << ": " << int(bits.a) << "+" << int(shift.a) << std::endl;

            //------------------------------------------------------------------
            // deduce bits per pixel
            //------------------------------------------------------------------
            uint8_t msb[4] = {
                bits.r + shift.r,
                bits.g + shift.g,
                bits.b + shift.b,
                bits.a + shift.a
            };
            c_sort(msb,4);
            std::cerr << "highest bits@" << int(msb[3]) << std::endl;
            (uint8_t &)bits_per_pixel = msb[3];
            
            if(bits_per_pixel>bytes_per_pixel*8)
                throw exception("Invalid bits_per_pixel=%u for bytes_per_pixel=%u", unsigned(bits_per_pixel), unsigned(bytes_per_pixel));
            
            //------------------------------------------------------------------
            // hook C-functions
            //------------------------------------------------------------------
            switch( bytes_per_pixel )
            {
                case 1:
                    set_pixel_ = set_pixel8;
                    get_pixel_ = get_pixel8;
                    break;
                    
                case 2:
                    set_pixel_ = set_pixel16;
                    get_pixel_ = get_pixel16;
                    break;
                    
                case 3:
                    set_pixel_ = set_pixel24;
                    get_pixel_ = get_pixel24;
                    break;
                    
                case 4:
                    set_pixel_ = set_pixel32;
                    get_pixel_ = get_pixel32;
                    break;
                    
                default:
                    throw exception("unhandled bytes_per_pixel=%u", unsigned(bytes_per_pixel));
            }
        }
        
        
               
        color_format:: color_format(uint8_t  BPP,
                                    uint32_t rmask,
                                    uint32_t gmask,
                                    uint32_t bmask,
                                    uint32_t amask) :
        get_pixel_(0),
        set_pixel_(0),
        mask(rmask,gmask,bmask,amask),
        bits(),
        shift(),
        loss(),
        bytes_per_pixel(BPP),
        bits_per_pixel(0)
        {
            get_metrics();
        }
        
        color_format:: color_format(const color_format &fmt) throw():
        get_pixel_( fmt.get_pixel_ ),
        set_pixel_( fmt.set_pixel_ ),
        mask(  fmt.mask ),
        bits(  fmt.bits ),
        shift( fmt.shift ),
        loss(  fmt.loss ),
        bytes_per_pixel( fmt.bytes_per_pixel ),
        bits_per_pixel(  fmt.bits_per_pixel )
        {
        }
        
        
        
        uint32_t color_format:: get_pixel( const void *addr ) const throw()
        {
            assert(get_pixel_);
            assert(addr);
            return get_pixel_(addr);
        }
        
        
        void color_format:: set_pixel( uint32_t c, void *addr) const throw()
        {
            assert(set_pixel_);
            assert(addr);
            set_pixel_(c,addr);
        }
        
#define Y_COLOR_GET(field) ( (c&mask.field) >> shift.field )
        
        quad8_t color_format:: get_color4(uint32_t c) const throw()
        {
            const uint32_t R = Y_COLOR_GET(r);
            const uint32_t G = Y_COLOR_GET(g);
            const uint32_t B = Y_COLOR_GET(b);
            const uint32_t A = Y_COLOR_GET(a);
            return quad8_t(R,G,B,A);
        }
        
        quad8_t color_format:: get_color3(uint32_t c) const throw()
        {
            
            const uint32_t R = Y_COLOR_GET(r);
            const uint32_t G = Y_COLOR_GET(g);
            const uint32_t B = Y_COLOR_GET(b);
            return quad8_t(R,G,B,0xff);
        }


#define Y_COLOR_MAP(u,field) ( (uint32_t(u) >> loss.field) << shift.field )
        
        uint32_t color_format:: map_color3( uint8_t r, uint8_t g, uint8_t b ) const throw()
        {
            const uint32_t R = Y_COLOR_MAP(r,r);
            const uint32_t G = Y_COLOR_MAP(g,g);
            const uint32_t B = Y_COLOR_MAP(b,b);
            const uint32_t A = Y_COLOR_MAP(0xff,a);

            return R|G|B|A;
        }

        uint32_t color_format:: map_color4( uint8_t r, uint8_t g, uint8_t b, uint8_t a ) const throw()
        {
            const uint32_t R = Y_COLOR_MAP(r,r);
            const uint32_t G = Y_COLOR_MAP(g,g);
            const uint32_t B = Y_COLOR_MAP(b,b);
            const uint32_t A = Y_COLOR_MAP(a,a);
            
            return R|G|B|A;
        }
        
        
        color_format color_format:: RGBA32()
        {
            return color_format(4,0xff000000,0x00ff0000,0x0000ff00,0x000000ff);
        }
        
        color_format color_format:: ARGB32()
        {
            return color_format(4,0x00ff0000,0x0000ff00,0x000000ff,0xff000000);
        }
        
        color_format color_format:: RGB24()
        {
            return color_format(3,0x00ff0000,0x0000ff00,0x000000ff,0x00000000);
        }
        
        color_format color_format:: RGBA16()
        {
            return color_format(2,0xf000,0x0f00,0x00f0,0x000f);
        }
        
    }
    
}
