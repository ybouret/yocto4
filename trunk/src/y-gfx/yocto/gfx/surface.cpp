#include "yocto/gfx/surface.hpp"

namespace yocto
{
    namespace gfx
    {
        
        surface * surface:: create(const format fmt, unit_t W, unit_t H)
        {
            return new surface(fmt,W,H);
        }
        
        surface:: surface( const format &fmt, unit_t W, unit_t H ) :
        format(fmt),
        bitmap(fmt.bytes,W,H)
        {
        }
        
        surface:: ~surface() throw()
        {
        }
        
    }
    
}
