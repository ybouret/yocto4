#ifndef YOCTO_FAME_SPLIT_QUAD_GHOSTS_INCLUDED
#define YOCTO_FAME_SPLIT_QUAD_GHOSTS_INCLUDED 1

#include "yocto/fame/ghost.hpp"
#include "yocto/fame/layout.hpp"

namespace yocto
{
    namespace fame
    {
     
        template <size_t DIM>
        class quad_ghost : public ghost
        {
        public:
            typedef typename layout_for<DIM>::type LAYOUT;
            typedef typename LAYOUT::coord         COORD;
            
            
            inline quad_ghost( const LAYOUT &outline, const LAYOUT &zone ) : ghost( zone.items )
            {
                outline.store_offsets_of(zone,*this);
            }
            
            virtual ~quad_ghost() throw()
            {
                
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(quad_ghost);
        };
        
        
        
        template <size_t DIM>
        class local_quad_ghosts
        {
        public:
            
        private:
            
        };
        
        
    }
}

#endif
