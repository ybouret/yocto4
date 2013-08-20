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
            
            const size_t  bytes_per_pixel;
            pixel_t     (*get_pixel)(const void *);
            void        (*put_pixel)(void *,pixel_t);
            
            static void fmt(const uint32_t  mask,
                            uint8_t       &bits,
                            uint8_t       &shift,
                            uint8_t       &loss,
                            const char    *field);
            
        private:
            YOCTO_DISABLE_ASSIGN(metrics);
        };
    }
}

#endif

