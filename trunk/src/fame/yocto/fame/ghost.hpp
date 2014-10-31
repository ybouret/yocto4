#ifndef YOCTO_FAME_GHOST_INCLUDED
#define YOCTO_FAME_GHOST_INCLUDED 1

#include "yocto/fame/linear.hpp"

namespace yocto
{
    namespace fame
    {
        
        class ghost : public offsets_list
        {
        public:
            explicit ghost(size_t n);
            virtual ~ghost() throw();
            
            
            static void copy(linear_space       &dst,
                             const ghost        &gdst,
                             const linear_space &src,
                             const ghost        &gsrc) throw();
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(ghost);
        };
        
    }
}

#endif
