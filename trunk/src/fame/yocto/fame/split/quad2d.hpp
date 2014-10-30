#ifndef YOCTO_FAME_SPLIT_QUAD2D_INCLUDED
#define YOCTO_FAME_SPLIT_QUAD2D_INCLUDED 1

#include "yocto/fame/split/quad-links.hpp"
#include "yocto/fame/split/quad-ghosts.hpp"

namespace yocto
{
    
    namespace fame
    {
        struct quad2D
        {
            typedef local_quad_ghosts<2> local_ghosts;
            
            static
            layout2D split(const layout2D &full,
                           const int       rank,
                           const int       size,
                           const bool      xpbc,
                           quad_links     &xlinks,
                           const bool      ypbc,
                           quad_links     &ylinks,
                           coord2D        &ranks,
                           coord2D        &sizes);
            
            static
            layout2D outline_of(const layout2D     &sub,
                                const int           rank,
                                const quad_links   &xlinks,
                                const quad_links   &ylinks,
                                const int           ng,
                                local_ghosts::list &lg);
        };
        
    }
    
}

#endif

