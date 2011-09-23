#include "yocto/rx/syntactic/rule.hpp"

namespace yocto
{
	
	namespace regex
	{
		namespace syntactic
		{

			rule:: ~rule() throw() {}
			
			rule:: rule( rule *p, const string &id ) : parent(p),name( id ) {}
			
			
			
		}

	}

}
