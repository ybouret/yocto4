#include "yocto/spade/types.hpp"

namespace yocto
{
    namespace spade
    {
        offsets_list:: ~offsets_list() throw()
        {
        }
        
        offsets_list:: offsets_list() throw() : sorted_offsets() {}
        
        offsets_list:: offsets_list( size_t num ) : sorted_offsets( num, as_capacity)
        {
        }
        
        offsets_list:: offsets_list( const offsets_list &other ) : sorted_offsets( other )
        {
        }
        
        void offsets_list:: store( size_t indx )
        {
            (void) insert(indx);
        }

    }
    
}

