#include "yocto/spade/async-ghosts.hpp"

namespace yocto
{
    
    namespace spade
    {
        
        async_ghosts:: ~async_ghosts() throw()
        {
        }
        
        async_ghosts:: async_ghosts( ghost::position pos ) throw() :
        inner( pos ),
        outer( pos ),
        peer(-1)
        {
        }
        
        
    }
    
}
