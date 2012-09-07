#ifndef YOCTO_SPADE_GHOST_INCLUDED
#define YOCTO_SPADE_GHOST_INCLUDED 1

#include "yocto/spade/types.hpp"

namespace yocto
{
    namespace spade
    {
        
        //! a ghost is a localized list of offsets...
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
            
            const position site;    //!< ghost position
            offsets_list   offsets; //!< initial empty list
            
            static const char     *get_position_name( ghost::position p ) throw();
            static ghost::position get_mirror_position( ghost::position p ) throw();
            static ghost::position get_lower_position( size_t dim );
            static ghost::position get_upper_position( size_t dim );
            
            ~ghost() throw();
            
                       
            //! ctor: set ghost position
            explicit ghost( ghost::position p ) throw();
            
            //! get_position_name(site)
            const char *position_name() const throw();
            
            //! get mirror position if needed
            ghost::position mirror_position() const throw();

            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(ghost);
            
        };
        
    }
}

#endif
