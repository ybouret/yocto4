#ifndef YOCTO_FAME_SPLIT_QUAD2D_INCLUDED
#define YOCTO_FAME_SPLIT_QUAD2D_INCLUDED 1

#include "yocto/fame/split/quad-links.hpp"
#include "yocto/fame/layout.hpp"

namespace yocto
{
    
    namespace fame
    {
        struct quad2D
        {
            
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

