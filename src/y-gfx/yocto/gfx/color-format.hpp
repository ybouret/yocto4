#ifndef YOCTO_GFX_COLOR_FORMAT_INCLUDED
#define YOCTO_GFX_COLOR_FORMAT_INCLUDED 1

#include "yocto/gfx/types.hpp"

namespace yocto
{
    namespace gfx
    {
        
        class color_format
        {
        public:
            ~color_format() throw();
            color_format(uint8_t  BPP,
                         uint32_t rmask,
                         uint32_t gmask,
                         uint32_t bmask,
                         uint32_t amask);
        
            const quad32  mask;
            const quad8   bits;   //!< #bits for each channel
            const quad8   shift;  //!< #left shifts for each channel
            const quad8   loss;   //!< #lost bits for each channel
            const uint8_t bytes_per_pixel; //!< compatibility is checked
            const uint8_t bits_per_pixel;  //!< from mask/bits
            
        private:
            void check_masks() const;
            void get_metrics(); //!< compute bits/shift/loss from mask
            YOCTO_DISABLE_ASSIGN(color_format);
        };
        
    }
}

#endif
