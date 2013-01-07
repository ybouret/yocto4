#include "yocto/error.hpp"



#include <cstdio>
#include <cstring>
#include <cctype>
#include <cstdlib>

#if defined(YOCTO_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#if defined(_MSC_VER)
#define snprintf _snprintf
#endif

#endif

#if defined(YOCTO_BSD)
#include <cerrno>
#endif

#include <iostream>

namespace yocto 
{
	
	namespace
	{
		static inline void clear_buffer( char *buffer ) throw()
		{
			size_t len = strlen( buffer );
			while( len-- > 0 && isspace( buffer[len] ) ) buffer[len] = 0;
		}
	}
	
	namespace libc
	{
#if defined(_MSC_VER)
#pragma warning (disable:4996)
#endif
		void format_error( char buffer[], size_t length, int err ) throw()
		{
			assert( !( buffer==NULL && length>0) );
			if( buffer )
			{
				memset(  buffer, 0, length );
				strncpy( buffer, strerror( err ), length );
				clear_buffer(buffer);
			}
			
		}
		
		void critical_error( int err, const char when[] ) throw()
		{
			const char *msg = when ? when : "?";
			char buffer[256];
			format_error(buffer, sizeof(buffer), err);
			std::cerr << "** Libc CRITICAL ERROR @: " << msg  << std::endl;
			std::cerr << "** '" << buffer << "'" << std::endl;
			exit(-1);
		}
	}
	
	namespace win32
	{
		void format_error( char buffer[], size_t length, uint32_t err ) throw()
		{
			assert( !( buffer==NULL && length>0) );
			if( buffer )
			{
				memset(buffer,0,length);
#if defined(YOCTO_WIN)
				FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM,
							  NULL,
							  err,
							  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
							  buffer,
							  length-1,
							  NULL );
				clear_buffer(buffer);
#endif
				if( strlen(buffer) == 0 )
				{
					snprintf(buffer,length-1,"[Error #%u]", (unsigned)err);
				}
				
			}
		}
		
		
		void critical_error( uint32_t err, const char when[] ) throw()
		{
			const char *msg = when ? when : "?";
			char buffer[256];
			format_error(buffer, sizeof(buffer), err);
			
			std::cerr << "** WIN32 CRITICAL ERROR @: " << msg  << std::endl;
			std::cerr << "** '" << buffer << "'" << std::endl;
			exit(-1);
		}
	}
	
	void format_error( char buffer[], size_t length, error_type err ) throw()
	{
#if defined(YOCTO_BSD)
		libc::format_error(buffer,length,err);
#endif
		
#if defined(YOCTO_WIN)
		win32::format_error(buffer,length,err);
#endif
	}
	
	void critical_error( error_type err, const char when[] ) throw()
	{
#if defined(YOCTO_BSD)
		libc::critical_error( err, when );
#endif
		
#if defined(YOCTO_WIN)
		win32::critical_error( err, when );
#endif
	}
    
#if defined(YOCTO_BSD)
    const error_type error_invalid_data = EINVAL;
#endif
    
#if defined(YOCTO_WIN)
    const error_type error_invalid_data = ERROR_INVALID_DATA;
#endif
}

