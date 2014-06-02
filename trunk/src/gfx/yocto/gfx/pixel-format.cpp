#include "yocto/gfx/pixel-format.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/exceptions.hpp"

#include <iostream>

namespace yocto  {
    
    namespace gfx
    {
        
        namespace
        {
            size_t __bytes_for( const uint32_t x ) throw()
            {
                if( x & 0xff000000 ) return 4;
                if( x & 0x00ff0000 ) return 3;
                if( x & 0x0000ff00 ) return 2;
                return 1;
            }
            
            
            size_t __get_depth(uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask  ) throw()
            {
                size_t ans = 1;
                ans        = max_of(ans,__bytes_for(Rmask));
                ans        = max_of(ans,__bytes_for(Gmask));
                ans        = max_of(ans,__bytes_for(Bmask));
                ans        = max_of(ans,__bytes_for(Amask));
                return ans;
            }
            
            void __get_bits_and_shift(const uint8_t &bits_,
                                      const uint8_t &shift_,
                                      const uint32_t x,
                                      const char    *field)
            {
                static const uint32_t motif[8] =
                {
                    0x01,
                    0x03,
                    0x07,
                    0x0f,
                    0x1f,
                    0x3f,
                    0x7f,
                    0xff
                };
                assert(field);
                uint8_t &bits  = (uint8_t &)bits_;
                uint8_t &shift = (uint8_t &)shift_;
                bits = shift = 0;
                if(x)
                {
                    for(bits=1;bits<=8;++bits)
                    {
                        const uint32_t m         = motif[bits-1];
                        const uint8_t  max_shift = 32-bits;
                        for(shift=0;shift<=max_shift;++shift)
                        {
                            if( x == (m<<shift) )
                                return;
                        }
                    }
                    throw exception("invalid %s mask", field);
                }
            }
            
            void __check_mask(const uint32_t A, const char *fieldA, const uint32_t B, const char *fieldB )
            {
                if(A&B) throw exception("overlapping %s and %s mask", fieldA, fieldB);
            }
            
            
            
        }
        
        pixel_format:: pixel_format( uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask) :
        metrics( __get_depth(Rmask,Gmask,Bmask,Amask)),
        mask(Rmask,Gmask,Bmask,Amask),
        bits(),
        shift(),
        loss(),
        bpp(0)
        {
            static const char *field[4] = { "red", "green", "blue", "alpha" };
            
            __check_mask(mask.r,field[0],mask.g,field[1]);
            __check_mask(mask.r,field[0],mask.b,field[2]);
            __check_mask(mask.r,field[0],mask.a,field[3]);
            
            __check_mask(mask.g,field[1],mask.b,field[2]);
            __check_mask(mask.g,field[1],mask.a,field[3]);
            
            __check_mask(mask.b,field[2],mask.a,field[3]);
            
            
            __get_bits_and_shift(bits.r,shift.r,mask.r,field[0]);
            __get_bits_and_shift(bits.g,shift.g,mask.g,field[1]);
            __get_bits_and_shift(bits.b,shift.b,mask.b,field[2]);
            __get_bits_and_shift(bits.a,shift.a,mask.a,field[3]);
            
            {
                uint8_t       *tgt = (uint8_t *)&loss;
                const uint8_t *src = (const uint8_t *)&bits;
                for(int i=0;i<4;++i)
                {
                    tgt[i] = 8 - src[i];
                    (size_t &)bpp += src[i];
                }
            }
            
        }
        
        pixel_format:: ~pixel_format() throw()
        {
        }
        
        pixel_format:: pixel_format(const pixel_format &other ) throw() :
        metrics(other),
        mask( other.mask ),
        bits(other.bits),
        shift(other.shift),
        loss(other.loss),
        bpp(other.bpp)
        {
            
        }
        
        rgb_t pixel_format:: get_rgb(pixel_t C) const throw()
        {
            const pixel_t r = ((C&mask.r)>>shift.r) << loss.r;
            const pixel_t g = ((C&mask.g)>>shift.g) << loss.g;
            const pixel_t b = ((C&mask.b)>>shift.b) << loss.b;
            return rgb_t(uint8_t(r),uint8_t(g),uint8_t(b));
        }
        
        rgba_t pixel_format:: get_rgba(pixel_t C) const throw()
        {
            const pixel_t r = ((C&mask.r)>>shift.r) << loss.r;
            const pixel_t g = ((C&mask.g)>>shift.g) << loss.g;
            const pixel_t b = ((C&mask.b)>>shift.b) << loss.b;
            const pixel_t a = ((C&mask.a)>>shift.a) << loss.a;
            
            return rgba_t(uint8_t(r),uint8_t(g),uint8_t(b),uint8_t(a));
        }
        
        pixel_t pixel_format:: map_rgb(const rgb_t &C) const throw()
        {
            const pixel_t R = ( pixel_t(C.r)  >> loss.r ) << shift.r;
            const pixel_t G = ( pixel_t(C.g)  >> loss.g ) << shift.g;
            const pixel_t B = ( pixel_t(C.b)  >> loss.b ) << shift.b;
            const pixel_t A = ( pixel_t(0xff) >> loss.a ) << shift.a;
            return R | G | B | A;
        }
        
        pixel_t pixel_format:: map_rgb(const rgb_t &C, uint8_t a) const throw()
        {
            const pixel_t R = ( pixel_t(C.r)  >> loss.r ) << shift.r;
            const pixel_t G = ( pixel_t(C.g)  >> loss.g ) << shift.g;
            const pixel_t B = ( pixel_t(C.b)  >> loss.b ) << shift.b;
            const pixel_t A = ( pixel_t(a)    >> loss.a ) << shift.a;
            return R | G | B | A;
        }
        
        pixel_t pixel_format:: map_rgba(const rgba_t &C) const throw()
        {
            const pixel_t R = ( pixel_t(C.r)  >> loss.r ) << shift.r;
            const pixel_t G = ( pixel_t(C.g)  >> loss.g ) << shift.g;
            const pixel_t B = ( pixel_t(C.b)  >> loss.b ) << shift.b;
            const pixel_t A = ( pixel_t(C.a)  >> loss.a ) << shift.a;
            return R | G | B | A;
        }
        
        
        pixel_format RGB24()
        {
            return pixel_format(0x00ff0000,0x0000ff00,0x000000ff,0x00000000);
        }
        
        pixel_format ARGB32()
        {
            return pixel_format(0x00ff0000,0x0000ff00,0x000000ff,0xff000000);
        }
        
        pixel_format RGBA32()
        {
            return pixel_format(0xff000000,0x00ff0000,0x0000ff00,0x000000ff);
        }
        
        
        pixel_format RGBA16()
        {
            return pixel_format(0x0000f000,0x00000f00,0x000000f0,0x0000000f);
        }
        
        pixel_format ARGB16()
        {
            return pixel_format(0x00000f00,0x000000f0,0x0000000f,0x0000f000);
        }
        
        
        std::ostream & operator<<( std::ostream &os, const pixel_format &fmt )
        {
            os << "R     = " << conv::binary(fmt.mask.r) << std::endl;
            os << "G     = " << conv::binary(fmt.mask.g) << std::endl;
            os << "B     = " << conv::binary(fmt.mask.b) << std::endl;
            os << "A     = " << conv::binary(fmt.mask.a) << std::endl;
            os << "bits  = "
            << int(fmt.bits.r) << " "
            << int(fmt.bits.g) << " "
            << int(fmt.bits.b) << " "
            << int(fmt.bits.a) << std::endl;
            os << "loss  = "
            << int(fmt.loss.r) << " "
            << int(fmt.loss.g) << " "
            << int(fmt.loss.b) << " "
            << int(fmt.loss.a) << std::endl;
            os << "depth = " << fmt.depth << std::endl;
            os << "bpp   = " << fmt.bpp;
            
            return os;
        }
    }
    
}
