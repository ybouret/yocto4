#ifndef YOCTO_SPADE_GHOSTS_SETUP_INCLUDED
#define YOCTO_SPADE_GHOSTS_SETUP_INCLUDED

#include "yocto/spade/ghosts/ghost.hpp"

namespace yocto
{
    
    namespace spade
    {
        
        class ghosts_info
        {
        public:
            ghosts_info() throw();
            ~ghosts_info() throw();
            bool   active; //!< default: false
            size_t count;  //!< default: 0 (num_ghosts)
            int    peer;   //!< default: -1
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(ghosts_info);
        };
        
        
        //! encoding ghosts setup
        class ghosts_setup
        {
        public:
            explicit ghosts_setup() throw();
            virtual ~ghosts_setup() throw();
                        
            void set_local( dimension_t dim, size_t num_ghosts );

            void set_async( ghost::position pos, size_t num_ghosts, int peer );
            
            
            const ghosts_info *get_local() const throw();
            const ghosts_info *get_async() const throw();
            
        private:
            ghosts_info local[3]; //! on_x,on_y,on_z
            ghosts_info async[6]; //! at_lower_x,at_upper_x,at_lower_y,at_upper_y,at_lower_z,at_upper_z
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(ghosts_setup);            
        };
        
    }
    
}

#endif
