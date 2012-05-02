#ifndef YOCTO_SWAMP_GHOSTS_INCLUDED
#define YOCTO_SWAMP_GHOSTS_INCLUDED 1

#include "yocto/swamp/layout.hpp"

namespace yocto 
{
    namespace swamp 
    {
        
        class ghost 
        {
        public:
            enum position
            {
                at_lower_x = 0,
                at_upper_x = 1,
                at_lower_y = 2,
                at_upper_y = 3,
                at_lower_z = 4,
                at_upper_z = 5
            };
            
            virtual ~ghost() throw();
            
        protected:
            explicit ghost() throw();
            offsets_list offsets;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(ghost);
        };
        
        
        
        
    }
}

#endif
