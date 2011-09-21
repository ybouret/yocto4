#include "yocto/rx/syntactic/rule.hpp"

namespace yocto
{
	
	namespace regex
	{
		namespace syntactic
		{

			rule:: ~rule() throw() {}
			
			rule:: rule( const string &id ) : name( id ) {}
			
			rule:: rule( const rule &other ) : name( other.name ) {}
			
			
		}

	}

}
