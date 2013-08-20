#ifndef YOCTO_COLOR_FORMAT_INCLUDED
#define YOCTO_COLOR_FORMAT_INCLUDED 1

#include "yocto/gfx/metrics.hpp"
#include "yocto/gfx/rgb.hpp"


namespace yocto
{
    namespace gfx
    {
        class color_format : public metrics
        {
        public:
            const rgb_mask_t mask;
            const rgb_t      bits;
            const rgb_t      shift;
            const rgb_t      loss;
            const size_t     bits_per_pixel;

            explicit color_format(size_t  BytesPerPixel,
                                  color_t Rmask,
                                  color_t Gmask,
                                  color_t Bmask,
                                  color_t Amask
                                  );
            
            virtual ~color_format() throw();
            color_format( const color_format &) throw();
            
            
            
        private:
            YOCTO_DISABLE_ASSIGN(color_format);
        };
    }
}

#endif
