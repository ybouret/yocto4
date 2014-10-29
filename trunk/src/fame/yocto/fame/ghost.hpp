#ifndef YOCTO_FAME_GHOST_INCLUDED
#define YOCTO_FAME_GHOST_INCLUDED 1

#include "yocto/fame/types.hpp"

namespace yocto
{
    namespace fame
    {
        
        class ghost : public offsets_list
        {
        public:
            explicit ghost() throw();
            explicit ghost(size_t n);
            virtual ~ghost() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(ghost);
        };
        
    }
}

#endif
