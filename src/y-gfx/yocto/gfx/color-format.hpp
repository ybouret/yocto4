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
            typedef uint32_t (*get_pixel_proc)( const void * );
            typedef void     (*set_pixel_proc)( uint32_t c, void *);
            
            ~color_format() throw();
            color_format(uint8_t  BPP,
                         uint32_t rmask,
                         uint32_t gmask,
                         uint32_t bmask,
                         uint32_t amask);
           
            color_format(const color_format &fmt) throw();
            
            static color_format RGBA32();
            static color_format ARGB32();
            static color_format RGB24();
            static color_format RGBA16();
            
            
        private:
            get_pixel_proc get_pixel_;
            set_pixel_proc set_pixel_;
            
        public:
            const quad32_t  mask;            //!< color mask for each channel
            const quad8_t   bits;            //!< #bits for each channel
            const quad8_t   shift;           //!< #left shifts for each channel
            const quad8_t   loss;            //!< #lost bits for each channel
            const size_t    bytes_per_pixel; //!< compatibility is checked
            const size_t    bits_per_pixel;  //!< from mask/bits
            
            uint32_t get_pixel( const void *addr ) const throw();
            void     set_pixel( uint32_t, void *addr) const throw();
            
            quad8_t  get_color3( uint32_t c ) const throw(); //!< r,g,b, a=0xff
            quad8_t  get_color4( uint32_t c ) const throw(); //!< r,g,b,a
          
            uint32_t map_color3( uint8_t r, uint8_t g, uint8_t b ) const throw();
            uint32_t map_color4( uint8_t r, uint8_t g, uint8_t b, uint8_t a ) const throw();
            
            uint32_t map_color3(quad8_t q) const throw();
            uint32_t map_color4(quad8_t q) const throw();
            
        private:
            
            void check_masks() const;
            void get_metrics();
            YOCTO_DISABLE_ASSIGN(color_format);
        };
        
        
        
    }
}

#endif
