#include "yocto/ios/ocstream.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>

#include <iostream>

namespace yocto
{

	namespace ios
	{

		ocstream:: ~ocstream() throw() {}

		ocstream:: ocstream( const char   *filename, bool append ) : cfile( filename, append ? "ab" : "wb" ) {}
		ocstream:: ocstream( const string &filename, bool append ) : cfile( filename.c_str(), append ? "ab" : "wb" ) {}
		ocstream:: ocstream( const cstdout_t &x ) : cfile(x) {}
		ocstream:: ocstream( const cstderr_t &x ) : cfile(x) {}

		
		void ocstream:: write( char C )
		{
			if( 1 != fwrite( &C, 1, 1, handle ) )
				throw libc::exception( errno, "ocstream::write(%d)", int(C) );

		}
		
		void ocstream:: flush() 
		{
			if( fflush(handle) != 0 )
			{
				throw libc::exception( EIO, "ocstream::flush()");
			}
		}
		
	}

}