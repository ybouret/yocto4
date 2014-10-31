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
            
            
        };
        
    }
    
}

#endif

