#include "yocto/rx/syntax/rule.hpp"

namespace yocto
{
	namespace regex
	{
		
		namespace syntax
		{
			
			rule:: ~rule() throw() {}
			
			rule:: rule( uint32_t t, const string &n ) :
			type(t),
			name(n),
			nref_(0)
			{
			}
			
			
		}

	}

}
