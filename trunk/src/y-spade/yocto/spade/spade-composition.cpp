#include "yocto/spade/composition.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
	
	namespace spade
	{
		
		composition:: ~composition() throw()
		{
		}
		
		composition:: composition(unit_t lo, 
								  unit_t hi ) throw() :
		index_min( min_of(lo,hi) ),
		index_max( max_of(lo,hi) ),
		components( 1 + (index_max-index_min) )
		{
		}
		
		composition:: composition( const composition &other ) throw() :
		index_min(  other.index_min  ),
		index_max(  other.index_max  ),
		components( other.components )
		{
		}
		
		
		
	}
	
}

