#include "yocto/exception.hpp"

#include <cstring>
#include <cstdarg>
#include <cstdio>

namespace yocto
{
#if defined(_MSC_VER)
#pragma warning ( disable : 4351 )
#endif

	exception:: exception() throw() : when_()
	{
		memset( when_, 0, sizeof(when_) );
	}

	exception:: exception( const exception &other ) throw() :
		when_()
	{
		memcpy( when_, other.when_, sizeof(when_) );
	}

	exception:: ~exception() throw()
	{
	}

	void exception:: format( const char *fmt, void *va_list_ptr ) throw()
    {
#if defined(_MSC_VER)
#pragma warning( disable : 4996 )
#endif
		assert(fmt);
        assert(va_list_ptr);
		va_list &ap = *static_cast<va_list *>(va_list_ptr);
		vsnprintf( when_, sizeof(when_)-1, fmt, ap);
		va_end (ap);
	}

	 exception:: exception( const char *fmt, ... ) throw() :
		when_()
	{
		memset( when_, 0, sizeof(when_) );
		va_list ap;
		va_start(ap, fmt);
		format( fmt, &ap );
	}

	void exception:: cat( const char *fmt,...) throw()
	{
		va_list   ap;
		va_start (ap, fmt);
		const size_t wlen = strlen( when_ );
		if( wlen < (sizeof(when_)-1) )
		{
			vsnprintf( when_+wlen,( (sizeof(when_)-1) - wlen ),fmt,ap );
		}
		va_end(ap);
	}

	void exception:: set( const char *fmt,...) throw()
	{
		memset( when_, 0, sizeof(when_) );
		va_list ap;
		va_start(ap, fmt);
		format( fmt, &ap );
	}
	
	void exception:: hdr( const char *fmt,...) throw()
	{
		char buff[ sizeof(when_) ];
		memcpy( buff, when_, sizeof(when_) );
		va_list   ap;
		va_start (ap, fmt);
		format( fmt, &ap );
		const size_t blen = strlen(buff);
		size_t       wlen = strlen( when_ );
		for( size_t i=0; i < blen && wlen < ( sizeof(when_) - 1 ); ++i )
		{
			when_[ wlen++ ] = buff[i];
		}
		
	}
	
	const char *exception:: what() const throw()
	{
		return "exception";
	}

	const char *exception:: when() const throw()
	{
		return when_;
    }



}
