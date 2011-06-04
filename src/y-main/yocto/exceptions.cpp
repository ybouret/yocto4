#define _CRT_SECURE_NO_WARNINGS

#include "yocto/exceptions.hpp"
#include "yocto/error.hpp"

#include <cstdarg>
#include <cstring>

namespace yocto {

	namespace libc {
#if defined(_MSC_VER)
#pragma warning ( disable : 4351 )
#endif
		exception:: exception( int err, const char *fmt, ... ) throw() :
				yocto::exception(),
				code_( err ),
				what_(     )
		{
			va_list ap;
			va_start(ap,fmt);
			format(fmt,&ap);
			libc::format_error( what_, sizeof(what_), code_ );
		}

		exception:: ~exception() throw()
		{
		}

		exception:: exception( const exception &other ) throw() :
				yocto::exception( other ),
				code_( other.code_ ),
				what_( )
		{
			memcpy( what_, other.what_, sizeof(what_) );
		}

		int exception:: code() const throw()
		{
			return code_;
		}

		const char *exception:: what() const throw()
		{
			return what_;
		}

	}

	namespace win32 {

		exception:: exception( uint32_t err, const char *fmt, ... ) throw() :
				yocto::exception(),
				code_( err ),
				what_(     )
		{
			va_list ap;
			va_start(ap,fmt);
			format(fmt,&ap);
			win32::format_error( what_, sizeof(what_), code_ );
		}

		exception:: ~exception() throw()
		{
		}

		exception:: exception( const exception &other ) throw() :
				yocto::exception( other ),
				code_( other.code_ ),
				what_( )
		{
			memcpy( what_, other.what_, sizeof(what_) );
		}

		uint32_t exception:: code() const throw()
		{
			return code_;
		}

		const char *exception:: what() const throw()
		{
			return what_;
		}

	}


	namespace imported {

		exception:: exception( const char *reason, const char *fmt,...) throw() :
				yocto::exception(),
				what_()
		{
			memset( what_, 0, sizeof(what_) );
			if( reason )
				strncpy( what_, reason, sizeof(what_)-1 );
			va_list ap;
			va_start(ap,fmt);
			format(fmt,&ap);
		}

		exception:: ~exception() throw()
		{
		}

		exception:: exception( const exception &other ) throw() :
				yocto::exception( other ),
				what_()
		{
			memcpy( what_, other.what_, sizeof(what_) );
		}

		const char *exception:: what() const throw()
		{
			return what_;
		}
		
		exception & exception:: operator=( const yocto::exception &other ) throw()
		{
			if( this != &other )
			{
				
				memset( what_, 0, sizeof(what_) );
				strncpy( what_, other.what(), sizeof(what_)-1 );
				set( "%s", other.when() );
			}
			return *this;
		}

		exception & exception:: operator=( const exception &other ) throw()
		{
			if( this != &other )
			{
				memcpy( what_, other.what_, sizeof(what_) );
				memcpy( when_, other.when_, sizeof(when_) );
			}
			return *this;
		}
		
	}


}
