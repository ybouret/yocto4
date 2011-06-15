#include "yocto/chars.hpp"

#include <cstring>

namespace yocto
{
	
	char   make_visible( char C ) throw()
	{
		if( C >= 32 && C < 127 ) return C;
		return '.';
	}
	
	size_t length_of( const char *s ) throw() { return s ? strlen(s) : 0; }
	
	size_t length_of( const utf8_t *s ) throw()
	{
		if( s )
		{
			size_t len = 0;
			while( *(s++) != 0 ) ++len;
			return len;
		}
		else
			return 0;
	}
	
}
