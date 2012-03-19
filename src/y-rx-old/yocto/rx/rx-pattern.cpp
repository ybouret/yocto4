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
		
		void pattern:: clear() throw()
		{
			this->release();
		}
		
		void pattern:: optimize() 
		{
		}
		
		
			
	}
}

