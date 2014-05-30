#ifndef YOCTO_PIXEL_FORMAT_INCLUDED
#define YOCTO_PIXEL_FORMAT_INCLUDED 1

#include "yocto/gfx/metrics.hpp"
#include "yocto/gfx/rgb.hpp"


namespace yocto
{
    namespace gfx
    {
        class pixel_format : public metrics
        {
        public:
            const rgb_mask_t mask;
            const rgb_t      bits;
            const rgb_t      shift;
            const rgb_t      loss;
            const pixel_t    opaque;
            const size_t     bpp;     //!< used bits of depth
            
            explicit pixel_format(size_t  Depth,
                                  pixel_t Rmask,
                                  pixel_t Gmask,
                                  pixel_t Bmask,
                                  pixel_t Amask
                                  );
            
            virtual ~pixel_format() throw();
            pixel_format( const pixel_format &) throw();
            
            pixel_t map_rgb(uint8_t r, uint8_t g, uint8_t b) const throw();
            pixel_t map_rgb(const rgb_t &c) const throw();
            
            
            pixel_t map_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) const throw();
            pixel_t map_rgba(const rgb_t &c) const throw();
            pixel_t map_rgba(const rgb_t &c, uint8_t a) const throw();
            
            rgb_t   get_rgb( pixel_t c) const throw();
            rgb_t   get_rgba(pixel_t c) const throw();
            
            
            static pixel_format ARGB32();
            static pixel_format ARGB16();
            static pixel_format RGB24();
            
        private:
            YOCTO_DISABLE_ASSIGN(pixel_format);
        };
    }
}

#endif
