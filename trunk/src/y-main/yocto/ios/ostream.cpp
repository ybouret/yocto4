#include "yocto/ios/ostream.hpp"
#include "yocto/exceptions.hpp"

#include <cerrno>


namespace yocto
{
	
	namespace ios
	{
		
		ostream:: ~ostream() throw() {}
		ostream:: ostream() throw()  {}
		
		size_t ostream:: put( const void *buffer, size_t buflen )
		{
			assert(!(NULL==buffer&&buflen>0) );
			const char *C = (const char *)buffer;
			for( size_t i=0; i < buflen; ++i ) write( *(C++) );
			return buflen;
		}
		
		void ostream:: save( const void *buffer, size_t buflen )
		{
			const size_t saved = put( buffer, buflen );
			if( saved < buflen )
				throw libc::exception( EIO, "ostream::save(%u < %u)", unsigned(saved), unsigned(buflen) );
		}

		
		
	}
	
}