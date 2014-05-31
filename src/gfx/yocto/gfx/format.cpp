#include "yocto/gfx/format.hpp"
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
        
        format:: format( uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask) :
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
        
        format:: ~format() throw()
        {
        }
        
        format:: format(const format &other ) throw() :
        metrics(other),
        mask( other.mask ),
        bits(other.bits),
        shift(other.shift),
        loss(other.loss),
        bpp(other.bpp)
        {
            
        }
        
    }
    
}
