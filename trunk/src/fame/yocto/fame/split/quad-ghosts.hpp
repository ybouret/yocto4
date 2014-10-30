#ifndef YOCTO_FAME_SPLIT_QUAD_GHOSTS_INCLUDED
#define YOCTO_FAME_SPLIT_QUAD_GHOSTS_INCLUDED 1

#include "yocto/fame/ghost.hpp"
#include "yocto/fame/layout.hpp"
#include "yocto/core/list.hpp"

namespace yocto
{
    namespace fame
    {
        
     
        template <size_t DIM>
        class quad_ghost : public ghost
        {
        public:
            typedef typename layout_for<DIM>::type Layout;
            //typedef typename LAYOUT::coord         Coord;
            
            
            inline quad_ghost(const Layout &outline,
                              const Layout &zone ) : ghost( zone.items )
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
        class local_quad_ghosts : public object
        {
        public:
            typedef local_quad_ghosts      Ghosts;
            typedef quad_ghost<DIM>        Ghost;
            typedef typename Ghost::Layout Layout;;
            
            const   quad_ghost<DIM> source;
            const   quad_ghost<DIM> target;
            
            local_quad_ghosts<DIM> *next;
            local_quad_ghosts<DIM> *prev;
            
            typedef core::list_of_cpp< local_quad_ghosts<DIM> > list;
            
            explicit local_quad_ghosts(const Layout outline,
                                       const Layout source_layout,
                                       const Layout target_layout):
            source(outline,source_layout),
            target(outline,target_layout),
            next(0),
            prev(0)
            {
                assert( source.size() == target.size() );
            }
            
            virtual ~local_quad_ghosts() throw()
            {
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(local_quad_ghosts);
        };
        
      
        
        
    }
}

#endif
