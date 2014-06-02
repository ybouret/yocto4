#include "yocto/gfx/surface.hpp"

namespace yocto
{
    namespace gfx
    {
        surface:: ~surface() throw()
        {
        }
        
        surface:: surface( const pixel_format fmt, unit_t width, unit_t height) :
        bitmap( fmt.depth, width, height ),
        format( fmt )
        {
        }
        
        
    }
}
