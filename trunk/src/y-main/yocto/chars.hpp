#ifndef YOCTO_CHARS_INCLUDED
#define YOCTO_CHARS_INCLUDED

#include "yocto/os.hpp"

namespace yocto
{
	typedef uint32_t utf8_t;
	
	char   make_visible( char ) throw();
	size_t length_of( const char   *s ) throw();
	size_t length_of( const utf8_t *s ) throw();
	
	template <typename T>
	struct character
	{
		static bool is_alpha( T c ) throw();
		static bool is_alnum( T c ) throw();
		static bool is_digit( T c ) throw();
		static bool is_xdigit( T c) throw();
		static bool is_space( T c ) throw();
		
		static T    to_upper( T c ) throw();
		static T    to_lower( T c ) throw();
	};
	
	
	
}

#endif
