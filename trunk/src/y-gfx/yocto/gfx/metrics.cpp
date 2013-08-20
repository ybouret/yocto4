#include "yocto/gfx/metrics.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace gfx
    {
        
        metrics:: ~metrics() throw() {}
        
        
        metrics:: metrics( const metrics &other ) throw() :
        bytes_per_pixel( other.bytes_per_pixel )
        {
        }
        
        metrics:: metrics(size_t BytesPerPixel) :
        bytes_per_pixel( BytesPerPixel )
        {
            if(bytes_per_pixel<=0)
                throw exception("gfx::metrics(invalid bytes_per_pixel)");
        }
        
    }
    
}
