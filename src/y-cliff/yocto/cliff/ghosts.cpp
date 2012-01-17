#include "yocto/cliff/ghosts.hpp"

namespace yocto 
{
	
	namespace cliff
	{
		
		ghosts_base:: ghosts_base( size_t max_offsets ) : offsets(max_offsets,as_capacity)
		{
		}
		
		ghosts_base:: ~ghosts_base() throw()
		{
		}
		
		
	}
	
	
}
