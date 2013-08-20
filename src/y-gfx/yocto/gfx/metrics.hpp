#ifndef YOCTO_GFX_METRICS_INCLUDED
#define YOCTO_GFX_METRICS_INCLUDED 1

#include "yocto/gfx/types.hpp"

namespace yocto
{
    namespace gfx
    {
        //! common color_format/pixel_buffer info
        class metrics
        {
        public:
            explicit metrics( size_t BPP );
            virtual ~metrics() throw();
            metrics( const metrics &) throw();
            
            const size_t bytes_per_pixel;
            
            
        private:
            YOCTO_DISABLE_ASSIGN(metrics);
        };
    }
}

#endif

