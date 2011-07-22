#include "yocto/rx/pattern.hpp"
#include "yocto/rx/source.hpp"

namespace yocto
{
	namespace regex
	{
		pattern:: pattern( uint32_t t ) throw() : 
		type(t), 
		data(NULL),
		next(NULL),
		prev(NULL)
		{
		}
		
		pattern:: ~pattern() throw() 
		{
		}
		
		void pattern:: sendto( source &src )
		{
			back_to( src.pool );
		}
		
			
	}
}

