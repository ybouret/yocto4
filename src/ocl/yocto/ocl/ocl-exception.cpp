#include "yocto/ocl/exception.hpp"

#include <cstdarg>
#include <cstring>

namespace yocto
{
	
    namespace ocl
    {
		
#if defined(_MSC_VER)
#	pragma warning ( disable : 4351 )
#endif
        Exception:: Exception( cl_int err, const char *fmt, ... ) throw() :
		exception(),
		code( err ),
		what_()
        {
            va_list ap;
            va_start(ap,fmt);
            format(fmt,&ap);
            memset(  what_, 0, sizeof(what_) );
#if defined(_MSC_VER)
#	pragma warning ( disable : 4996 )
#endif
            strncpy( what_, StrErr( code ), sizeof(what_)-1 );
        }
		
        Exception:: Exception( const Exception &other ) throw() :
		yocto::exception( other ),
		code( other.code ),
		what_( )
        {
            memcpy( what_, other.what_, sizeof(what_) );
        }
		
        Exception:: ~Exception() throw()
        {
        }
		
        const char * Exception:: what() const throw()
        {
            return what_ ;
        }
		
    }
}
