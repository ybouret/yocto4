#ifndef YOCTO_CONTAINER_ITERATOR_INCLUDED
#define YOCTO_CONTAINER_ITERATOR_INCLUDED 1


#include "yocto/os.hpp"

namespace yocto
{
	
	namespace iterating
	{
		enum direction
		{
			forward,
			reverse
		};
	}
    
    template <typename ITERATOR,typename FUNC>
    void for_each( ITERATOR cur, const ITERATOR end, FUNC &func )
    {
        for(;cur!=end;++cur)
        {
            func( *cur );
        }
    }
	
}

#endif
