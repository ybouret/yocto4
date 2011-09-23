#include "yocto/rx/parser.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		parser:: ~parser() throw()
		{
		}

		parser:: parser( syntactic::rule *root ) throw() : root_( root )
		{
			assert( root != NULL );
		}
		
	}
	
}