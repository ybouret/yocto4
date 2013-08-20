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
        opaque(other.opaque),
        bits_per_pixel(other.bits_per_pixel)
        {
            
        }
        
        
        
        color_format:: color_format(size_t  BytesPerPixel,
                                    pixel_t Rmask,
                                    pixel_t Gmask,
                                    pixel_t Bmask,
                                    pixel_t Amask) :
        metrics(BytesPerPixel),
        mask( Rmask, Gmask, Bmask, Amask),
        bits(0,0,0,0),
        shift(0,0,0,0),
        loss(0,0,0,0),
        opaque(0),
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
            
            //-- chek bits/bytes
            if( bits_per_pixel > 8 * bytes_per_pixel)
                throw exception("gfx::color_format(too many bits per pixel)");
            
            //-- compute opaque
            (pixel_t &) opaque = ( (pixel_t(0xff) >> loss.a) << shift.a );
        }
        
        pixel_t color_format:: map_rgb(uint8_t r, uint8_t g, uint8_t b) const throw()
        {
            return
            ( (pixel_t(r)    >> loss.r) << shift.r ) |
            ( (pixel_t(g)    >> loss.g) << shift.g ) |
            ( (pixel_t(b)    >> loss.b) << shift.b ) |
            opaque;
        }
        
        pixel_t color_format:: map_rgb(const rgb_t &c) const throw()
        {
            return map_rgb(c.r,c.g,c.b);
        }
        
        
        pixel_t color_format:: map_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) const throw()
        {
            return
            ( (pixel_t(r)    >> loss.r) << shift.r ) |
            ( (pixel_t(g)    >> loss.g) << shift.g ) |
            ( (pixel_t(b)    >> loss.b) << shift.b ) |
            ( (pixel_t(a)    >> loss.a) << shift.a );
        }
        
        pixel_t color_format:: map_rgba(const rgb_t &c, uint8_t a) const throw()
        {
            return map_rgba(c.r,c.g,c.b,a);
        }
        
        
    }
}