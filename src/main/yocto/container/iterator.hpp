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
    
    template <typename ITER,typename FUNC>
    void for_each( ITER cur, const ITER end, FUNC &func )
    {
        for(;cur!=end;++cur)
        {
            func( *cur );
        }
    }
	
}

#endif
