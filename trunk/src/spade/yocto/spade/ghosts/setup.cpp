#include "yocto/spade/ghosts/setup.hpp"
#include "yocto/exception.hpp"


namespace yocto
{
    
    namespace spade
    {
        
        ghosts_info:: ~ghosts_info() throw() {}
        ghosts_info::  ghosts_info() throw() :
        active(false),
        count(0),
        peer(-1)
        {
        }
        
        
        ghosts_setup:: ghosts_setup() throw() :
        local(),
        async()
        {
        }
        
        ghosts_setup::~ghosts_setup() throw()
        {
        }
        
        void ghosts_setup:: set_local( dimension_t dim, size_t num_ghosts )
        {
            const char D = char('x'+dim);
            assert(size_t(dim)<3);
            if( local[dim].active)
                throw exception("ghost_setup.set_local_%c: already set",D);
            
            const size_t at_async = dim << 1;
            if( async[at_async].active || async[at_async+1].active)
                throw exception("ghost_setup.set_local_%c: ASYNC already set",D);
            
            local[dim].active = true;
            local[dim].count  = num_ghosts;
            
        }
        
        void ghosts_setup:: set_async( ghost::position pos, size_t num_ghosts, int peer )
        {
            const size_t dim = pos >> 1;
            if( local[dim].active )
                throw exception("ghost_setup.set_async('%s'): LOCAL already active", ghost::get_position_name(pos));
            
            if( async[pos].active)
                throw exception("ghost_setup.set_async('%s'):  already active", ghost::get_position_name(pos));
            
            async[pos].active = true;
            async[pos].count  = num_ghosts;
            async[pos].peer   = peer;
            
        }
        
        const ghosts_info * ghosts_setup:: get_local() const throw()
        {
            return local;
        }
        
        const ghosts_info * ghosts_setup::get_async() const throw()
        {
            return async;
        }
        
    }
    
}
