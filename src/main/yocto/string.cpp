#define _CRT_SECURE_NO_WARNINGS 1
#include "yocto/string.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/exceptions.hpp"

#include <cerrno>
#include <cstdarg>
#include <cstdio>

namespace yocto
{

	
    
	string vformat( const char *fmt,... )
	{
		assert(fmt);
		size_t n = 32;
	TRY:
		{
			memory::buffer_of<char,memory::global> databuf( n );
			char  *buffer  = databuf();
			int    length  = int( databuf.length() );
			if( length < 0 )
				throw libc::exception( ERANGE, "string::vformat memory overflow");
			
			va_list ap;
			va_start( ap, fmt );
			const int  result  = vsnprintf( buffer, length, fmt, ap);
			const bool success = (result >= 0 && result < length); // WIN32 and GLIBC compatible
			va_end (ap);
			
			if( success )
			{
				return string( buffer );
			}
			n <<= 1;
		}
		goto TRY;
		
	}
	
}
