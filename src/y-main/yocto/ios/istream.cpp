#include "yocto/ios/istream.hpp"
#include "yocto/exceptions.hpp"

#include <cerrno>

namespace yocto
{
	
	namespace ios
	{
		
		istream:: ~istream() throw() {}
		istream:: istream() throw()  {}
		
		size_t istream:: get( void *buffer, size_t buflen )
		{
			assert( !(buffer==NULL&&buflen>0) );
			char  *C = static_cast<char*> (buffer);
			size_t n = 0;
			while( n < buflen )
			{
				if( !query( *(C++) ) ) return n;
				++n;
			}
			return n;
		}

		void istream:: load( void *buffer, size_t buflen )
		{
			const size_t loaded = get(buffer,buflen);
			if(  loaded < buflen )
				throw libc::exception( EIO, "istream::load( %u < %u )", unsigned(loaded), unsigned(buflen));
		}
		
	}
	
}