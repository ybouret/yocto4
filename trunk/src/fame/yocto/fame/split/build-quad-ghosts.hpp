#ifndef YOCTO_FAME_SPLIT_BUILD_QUAD_GHOSTS_INCLUDED
#define YOCTO_FAME_SPLIT_BUILD_QUAD_GHOSTS_INCLUDED 1

#include "yocto/fame/split/quad-links.hpp"
#include "yocto/fame/split/quad-ghosts.hpp"

namespace yocto
{
    
    namespace fame
    {
        
        template <size_t DIM>
        struct build_quad_ghosts
        {
            typedef typename layout_for<DIM>::type Layout;
            typedef typename Layout::coord         Coord;
            typedef  local_quad_ghosts<DIM>        LocalGhosts;
            typedef typename LocalGhosts::list     LocalGhostsList;
            
            static inline
            void on(const Layout     &inside,
                    const Layout     &outline,
                    const size_t      dim,
                    const quad_links &links,
                    LocalGhostsList  &lg)
            {
                lg.clear();
                
            }
            
            
            
            
        };
        
        
    }
}

#endif
