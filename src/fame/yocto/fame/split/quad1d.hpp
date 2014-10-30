#ifndef YOCTO_FAME_SPLIT_QUAD1D_INCLUDED
#define YOCTO_FAME_SPLIT_QUAD1D_INCLUDED 1

#include "yocto/fame/split/quad-links.hpp"
#include "yocto/fame/split/quad-ghosts.hpp"

namespace yocto
{
    
    namespace fame
    {
        
        struct quad1d
        {
            static
            layout1D split(const layout1D &full,
                           const int       rank,
                           const int       size,
                           const bool      pbc,
                           quad_links     &links );
            
            static
            layout1D outline_of(const layout1D   &sub,
                                const int         rank,
                                const quad_links &links,
                                const int         ng);
        };
        
        
    }
    
}

#endif

