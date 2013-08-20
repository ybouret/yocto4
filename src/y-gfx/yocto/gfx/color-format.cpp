#include "yocto/gfx/color-format.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace gfx
    {
        color_format:: ~color_format() throw() {}
        
        color_format:: color_format( const color_format &other ) throw() :
        metrics(other),
        mask(other.mask),
        bits(other.bits),
        shift(other.shift),
        loss(other.loss),
        bits_per_pixel(other.bits_per_pixel)
        {
            
        }
        
        
        
        color_format:: color_format(size_t  BytesPerPixel,
                                    color_t Rmask,
                                    color_t Gmask,
                                    color_t Bmask,
                                    color_t Amask) :
        metrics(BytesPerPixel),
        mask( Rmask, Gmask, Bmask, Amask),
        bits(0,0,0,0),
        shift(0,0,0,0),
        loss(0,0,0,0),
        bits_per_pixel(0)
        {
            static const char *name[4] = { "r", "g", "b", "a" };
            const uint32_t *mask_p = (uint32_t *)&mask.r;
            
            //-- check no overlap
            for(size_t i=0;i<4;++i)
            {
                for(size_t j=i+1;j<4;++j)
                {
                    if( mask_p[i] & mask_p[j] )
                        throw exception("gfx::color_format(overlapping mask '%s' and '%s'", name[i], name[j]);
                }
            }
            
            //-- compute fields
            uint8_t        *bits_p = (uint8_t *) &bits.r;
            uint8_t        *shft_p = (uint8_t *) &shift.r;
            uint8_t        *loss_p = (uint8_t *) &loss.r;
            for(size_t i=0; i <4; ++i )
            {
                metrics::fmt(mask_p[i],bits_p[i], shft_p[i], loss_p[i], name[i]);
            }
            
            //-- get bits per pixel
            size_t &bpp = (size_t &)bits_per_pixel;
            for(size_t i=0;i<4;++i)
            {
                size_t bmax = bits_p[i] + shft_p[i];
                if(bmax>bpp) bpp = bmax;
            }
            
            if( bits_per_pixel > 8 * bytes_per_pixel)
                throw exception("gfx::color_format(too many bits per pixel)");
        }
        
    }
}