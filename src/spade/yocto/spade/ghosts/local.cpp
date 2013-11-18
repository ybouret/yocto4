#include "yocto/spade/ghosts/local.hpp"

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
        
        local_ghosts:: local_ghosts( dimension_t dim ) throw() :
        lower( ghost::get_lower_position(dim) ),
        upper( ghost::get_upper_position(dim) )
        {
        }
        
        void local_ghosts:: transfer( linear &handle ) throw()
        {
            assert( lower.inside.size() == num_offsets );
            assert( lower.mirror.size() == num_offsets );
            assert( upper.inside.size() == num_offsets );
            assert( upper.mirror.size() == num_offsets );
            for( size_t i=num_offsets;i>0;--i)
            {
                handle.local_copy( lower.mirror[i], lower.inside[i]);
                handle.local_copy( upper.mirror[i], upper.inside[i]);
            }
        }
        
        void local_ghosts:: transfer( array<linear_ptr> &handles ) throw()
        {
            assert( lower.inside.size() == num_offsets );
            assert( lower.mirror.size() == num_offsets );
            assert( upper.inside.size() == num_offsets );
            assert( upper.mirror.size() == num_offsets );

            
            const size_t num_handles = handles.size();
            for( size_t i=num_offsets;i>0;--i)
            {
                const size_t lower_mirror = lower.mirror[i];
                const size_t lower_inside = lower.inside[i];
                const size_t upper_mirror = upper.mirror[i];
                const size_t upper_inside = upper.inside[i];
                
                for( size_t j=num_handles;j>0;--j)
                {
                    assert(handles[j]);
                    linear &handle = *handles[j];
                    handle.local_copy( lower_mirror, lower_inside);
                    handle.local_copy( upper_mirror, upper_inside);
                }
            }
        }
        
        
        
    }
    
}

