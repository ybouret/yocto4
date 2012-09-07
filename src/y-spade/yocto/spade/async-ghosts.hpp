#ifndef YOCTO_SPADE_ASYNC_GHOSTS_INCLUDED
#define YOCTO_SPADE_ASYNC_GHOSTS_INCLUDED 1

#include "yocto/spade/ghost.hpp"
#include "yocto/spade/linear.hpp"

namespace yocto
{
    namespace spade
    {
        
        class async_ghosts
        {
        public:
            //! set ghosts site
            explicit async_ghosts( ghost::position pos ) throw();
            
            //! release memory
            virtual ~async_ghosts() throw();
            
            ghost inner; //!< in layout
            ghost outer; //!< in outline
            int   peer;  //!< default is -1
            
            //! create offsets
            template <typename LAYOUT>
            inline void setup( size_t num_ghosts, const LAYOUT &outline, const LAYOUT &L)
            {
                //! create corresponding inner layout
                {
                    const LAYOUT inner_sub = inner.inner_sublayout(L, num_ghosts);
                    inner.load_from(outline, inner_sub);
                }
                
                //! create corresponding outer layout
                {
                    const LAYOUT outer_sub = outer.outer_sublayout(L,num_ghosts);
                    outer.load_from(outline,outer_sub);
                }
            }
            
            uint8_t *ibuffer; //!< buffer to store inner ghost
            uint8_t *obuffer; //!< buffer to query outer ghost
            size_t   iobytes; //!< maximum buffers capacity
            
        private:
            size_t iolen; //!< for memory allocation
            YOCTO_DISABLE_COPY_AND_ASSIGN(async_ghosts);
            
        };
        
    }
    
}

#endif

