#include "yocto/sequence/c-array.hpp"
#include "yocto/exception.hpp"


namespace yocto
{
	namespace hidden 
	{ 
		const char c_array_name[] = "c_array"; 
		void       c_array_reserve( const char *name )
		{
			assert( name );
			throw exception("%s.reserve: FORBIDDEN!", name);
		}
	}
}
