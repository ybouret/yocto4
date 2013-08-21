#include "yocto/gfx/surface.hpp"

namespace yocto
{
    namespace gfx
    {
        surface:: ~surface() throw()
        {
        }
        
        surface:: surface( const color_format fmt, size_t width, size_t height) :
        format(fmt),
        pixels( new pixbuf(fmt.bytes_per_pixel,width,height) ),
        w(width),
        h(height)
        {
            
        }
        
    }
}
