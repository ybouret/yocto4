#include "yocto/ios/ostream.hpp"
#include "yocto/exceptions.hpp"

#include <cerrno>


namespace yocto
{
	
	namespace ios
	{
		
		ostream:: ~ostream() throw() {}
		ostream:: ostream() throw()  {}
		
		void ostream:: put( const void *data, size_t size, size_t &done )
		{
			assert(!(NULL==data&&size>0) );
			const char *C = (const char *)data;
			for( done=0; done < size; ++done) write( *(C++) );
		}
		
		void ostream:: save( const void *buffer, size_t buflen )
		{
			size_t saved = 0;
			put( buffer, buflen, saved );
			if( saved < buflen )
				throw libc::exception( EIO, "ostream::save(%u < %u)", unsigned(saved), unsigned(buflen) );
		}

		
		
	}
	
}

