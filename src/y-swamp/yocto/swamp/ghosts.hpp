#ifndef YOCTO_SWAMP_GHOSTS_INCLUDED
#define YOCTO_SWAMP_GHOSTS_INCLUDED 1

#include "yocto/swamp/layout.hpp"

namespace yocto 
{
    namespace swamp 
    {
        
        class ghost_base 
        {
        public:
            virtual ~ghost_base() throw();
            
        protected:
            explicit ghost_base( size_t n );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(ghost_base);
        };
        
    }
}

#endif
