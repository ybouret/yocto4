#include "yocto/spade/local-ghosts.hpp"


namespace yocto
{
    namespace spade
    {
     
        
        ////////////////////////////////////////////////////////////////////////
        //
        // local ghost pair
        //
        ////////////////////////////////////////////////////////////////////////
        local_ghosts_pair:: ~local_ghosts_pair() throw()
        {
        }
        
        local_ghosts_pair:: local_ghosts_pair( ghost::position pos ) throw() :
        inside( pos ),
        mirror( inside.mirror_position() )
        {
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // local ghosts
        //
        ////////////////////////////////////////////////////////////////////////
        local_ghosts:: ~local_ghosts() throw() {}
        
        local_ghosts:: local_ghosts( size_t dim ) throw() :
        lower( ghost::get_lower_position(dim) ),
        upper( ghost::get_upper_position(dim) )
        {
        }
        
        void local_ghosts:: transfert( linear &handle ) throw()
        {
            assert( lower.inside.size() == lower.mirror.size() );
            assert( upper.inside.size() == lower.mirror.size() );
            assert( lower.inside.size() == upper.inside.size() );
            for( size_t i=lower.inside.size();i>0;--i)
            {
                handle.local_copy( lower.mirror[i], lower.inside[i]);
                handle.local_copy( upper.mirror[i], upper.inside[i]);
            }
        }

        
    }
    
}