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
            if(bytes_per_pixel<=0||bytes_per_pixel>4)
                throw exception("gfx::metrics(invalid bytes_per_pixel=%u)",unsigned(bytes_per_pixel));
        }
        
     
        void metrics:: fmt(const uint32_t mask,
                           uint8_t       &bits,
                           uint8_t       &shift,
                           uint8_t       &loss,
                           const char    *field)
        {
            assert(field);
            if(mask<=0)
            {
                bits  = 0;
                shift = 0;
                loss  = 8;
            }
            else
            {
                uint32_t w         = 0x00;
                uint8_t  shift_max = 32;
                for(bits=1; bits<=8; ++bits )
                {
                    // build the pattern
                    w <<= 0x01;
                    w  |= 0x01;
                    
                    // find the shift
                    --shift_max;
                    for(shift=0;shift<=shift_max;++shift)
                    {
                        if(mask == (w<<shift) )
                        {
                            loss = 8-bits;
                            return;
                        }
                    }
                }
                throw exception("gfx::metrics::fmt(invalid '%s' mask=0x%08x)", field, unsigned(mask) );
            }
        }
        
    }
    
}
