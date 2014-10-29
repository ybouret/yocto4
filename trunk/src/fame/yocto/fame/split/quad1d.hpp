#ifndef YOCTO_FAME_SPLIT_QUAD1D_INCLUDED
#define YOCTO_FAME_SPLIT_QUAD1D_INCLUDED 1

#include "yocto/fame/split/quad-links.hpp"
#include "yocto/fame/layout.hpp"

namespace yocto
{
    
    namespace fame
    {
        
        layout1D quad_split(const layout1D &full,
                            const int       rank,
                            const int       size,
                            const bool      pbc,
                            quad_links     &links );
        
    }
    
}

#endif

