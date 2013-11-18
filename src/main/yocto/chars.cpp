#include "yocto/chars.hpp"

#include <cstring>
#include <cctype>

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
	
#if defined(_MSC_VER)
#pragma warning ( disable : 4800 )
#endif
	
#define YOCTO_CHARACTER_TEST(FUNC) \
template <>               \
bool character<char>::is_##FUNC( char c ) throw() \
{ return is##FUNC(c); } \
template <>               \
bool character<utf8_t>::is_##FUNC( utf8_t w ) throw() \
{ return w < 128 ? is##FUNC(char(w)) : false; }
	
	YOCTO_CHARACTER_TEST(digit)
	YOCTO_CHARACTER_TEST(alpha)
	YOCTO_CHARACTER_TEST(xdigit)
	YOCTO_CHARACTER_TEST(alnum)
	YOCTO_CHARACTER_TEST(space)
	
#if 0
	template <>
	const char character<char>:: eof = EOF;
	
	template <>
	const Wchar character<Wchar>:: eof = ~Wchar(0);
#endif
	
	template <>
	char character<char>::to_upper( char c ) throw()
	{
		return char(toupper(c));
	}
	
	template <>
	utf8_t character<utf8_t>::to_upper(utf8_t w ) throw()
	{
		if( w < 128 )
		{
			return toupper( char(w) );
		}
		else
			return w;
	}
	
	template <>
	char character<char>::to_lower( char c ) throw()
	{
		return char(tolower(c));
	}
	
	template <>
	utf8_t character<utf8_t>::to_lower( utf8_t w ) throw()
	{
		if( w < 128 )
		{
			return tolower( char(w) );
		}
		else
			return w;
	}
	
}
