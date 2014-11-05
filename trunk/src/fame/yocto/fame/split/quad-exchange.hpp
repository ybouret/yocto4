#ifndef YOCTO_FAME_SPLIT_QUAD_EXCHANGE_INCLUDED
#define YOCTO_FAME_SPLIT_QUAD_EXCHANGE_INCLUDED 1

#include "yocto/fame/mesh/quad-grid.hpp"

namespace yocto
{
    namespace fame
    {
        
        struct quad_exchange
        {
            
            template <typename QUADGRID>  static inline
            void data(const QUADGRID &grid,
                      linear_handles &handles )
            {
                for(const typename QUADGRID::Ghosts *g = grid.local_ghosts.head;g;g=g->next)
                {
                    g->local_update(handles);
                }
            }
            
        };
        
    }
}

#endif
