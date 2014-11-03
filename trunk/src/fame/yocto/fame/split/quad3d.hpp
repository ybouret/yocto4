#ifndef YOCTO_FAME_SPLIT_QUAD3D_INCLUDED
#define YOCTO_FAME_SPLIT_QUAD3D_INCLUDED 1

#include "yocto/fame/layout.hpp"
#include "yocto/fame/split/quad-links.hpp"

namespace yocto
{
    namespace fame
    {
        
        struct quad3D
        {
            
            static layout3D split(const layout3D &full,
                                  const int       rank,
                                  const int       size,
                                  const bool      xpbc,
                                  quad_links     &xlinks,
                                  const bool      ypbc,
                                  quad_links     &ylinks,
                                  const bool      zpbc,
                                  quad_links     &zlinks,
                                  coord3D        &ranks,
                                  coord3D        &sizes);
            
        };
    }
}

#endif
