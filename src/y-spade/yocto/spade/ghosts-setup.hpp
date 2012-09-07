#ifndef YOCTO_SPADE_GHOSTS_SETUP_INCLUDED
#define YOCTO_SPADE_GHOSTS_SETUP_INCLUDED

#include "yocto/spade/ghost.hpp"

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
                        
            void set_local_x( size_t num_ghosts ); //!< for local x-PBC
            void set_local_y( size_t num_ghosts ); //!< for local y-PBC
            void set_local_z( size_t num_ghosts ); //!< for local z-PBC

            void set_async( ghost::position pos, size_t num_ghosts, int peer );
            
            
            const ghosts_info *get_local() const throw();
            const ghosts_info *get_async() const throw();
            
        private:
            ghosts_info local[3];
            ghosts_info async[6];
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(ghosts_setup);
            void set_local( size_t dim, char D, size_t num_ghost );
            
        };
        
    }
    
}

#endif
