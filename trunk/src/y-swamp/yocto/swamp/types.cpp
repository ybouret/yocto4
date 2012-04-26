#include "yocto/swamp/types.hpp"

namespace yocto 
{
    
    namespace swamp
    {
        offsets:: ~offsets() throw()
        {
        }
        
        offsets:: offsets() throw() : sorted_offsets() {}
        
        offsets:: offsets( size_t num ) : sorted_offsets( num, as_capacity)
        {
        }
        
        offsets:: offsets( const offsets &other ) : sorted_offsets( other )
        {
        }
        
        void offsets:: store( size_t indx )
        {
            (void) insert(indx);
        }

    }
    
}
