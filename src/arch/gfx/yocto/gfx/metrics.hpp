#ifndef YOCTO_GFX_METRICS_INCLUDED
#define YOCTO_GFX_METRICS_INCLUDED 1

#include "yocto/gfx/types.hpp"

namespace yocto
{
    namespace gfx
    {
        
        typedef pixel_t (*get_pixel_proc)(const void *);
        typedef void    (*put_pixel_proc)(void *,pixel_t);

        //! pack/unpack pixel
        class metrics
        {
        public:
            explicit metrics( size_t bytes_per_pixel );
            virtual ~metrics() throw();
            metrics(const metrics &other) throw();
            const size_t         depth;     //!< 1,2,3,4
            const get_pixel_proc get_pixel;
            const put_pixel_proc put_pixel;
            
        private:
            YOCTO_DISABLE_ASSIGN(metrics);
        };
    }
}

#endif
