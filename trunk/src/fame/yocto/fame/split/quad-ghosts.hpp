#ifndef YOCTO_FAME_SPLIT_QUAD_GHOSTS_INCLUDED
#define YOCTO_FAME_SPLIT_QUAD_GHOSTS_INCLUDED 1

#include "yocto/fame/ghost.hpp"
#include "yocto/fame/layout.hpp"
#include "yocto/core/list.hpp"

namespace yocto
{
    namespace fame
    {
        
        
        template <typename Layout>
        class quad_ghost : public ghost
        {
        public:
            const Layout zone;
            inline quad_ghost(const Layout &outline,
                              const Layout &gzone  ) :
            ghost(gzone.items),
            zone(gzone)
            {
                outline.store_offsets_of(zone,*this);
            }
            
            virtual ~quad_ghost() throw()
            {
                
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(quad_ghost);
        };
        
        
        
        
        template <typename Layout>
        class quad_ghosts : public object
        {
        public:
            typedef quad_ghosts        Ghosts;
            typedef quad_ghost<Layout> Ghost;
            
            const   int   peer;
            const   Ghost source;
            const   Ghost target;
            
            Ghosts *next;
            Ghosts *prev;
            
            typedef core::list_of_cpp< Ghosts > list;
            
            explicit quad_ghosts(const Layout outline,
                                 const int    peer_rank,
                                 const Layout source_layout,
                                 const Layout target_layout):
            peer(peer_rank),
            source(outline,source_layout),
            target(outline,target_layout),
            next(0),
            prev(0)
            {
                assert( source.size() == target.size() );
            }
            
            virtual ~quad_ghosts() throw()
            {
            }
            
            inline void local_update(linear_space       &tgt,
                                     const linear_space &src) const throw()
            {
                ghost::copy(tgt, target, src, source);
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(quad_ghosts);
        };
        
        
        
        
        
    }
}

#endif
