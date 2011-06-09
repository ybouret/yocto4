#ifndef YOCTO_CHARS_INCLUDED
#define YOCTO_CHARS_INCLUDED

#include "yocto/os.hpp"

namespace yocto
{
	typedef uint32_t utf8_t;
	
	size_t length_of( const char   *s ) throw();
	size_t length_of( const utf8_t *s ) throw();
	
	
}

#endif
