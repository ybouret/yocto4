#ifndef YOCTO_FAME_GHOST_QUAD_INCLUDED
#define YOCTO_FAME_GHOST_QUAD_INCLUDED 1

#include "yocto/fame/layout.hpp"
#include "yocto/fame/ghost.hpp"

namespace yocto
{
    namespace fame
    {
        
        // ghost in one direction
        class quad_ghost : public ghost
        {
        public:
            explicit quad_ghost();
            virtual ~quad_ghost() throw();
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(quad_ghost);
        };
        
    }
}

#endif
