#include "yocto/sequence/lw-array.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
	namespace hidden 
	{ 
		const char lw_array_name[] = "lw_array"; 
		void       lw_array_reserve( const char *name )
		{
			assert( name );
			throw exception("%s.reserve: FORBIDDEN!", name);
		}
	}
}
