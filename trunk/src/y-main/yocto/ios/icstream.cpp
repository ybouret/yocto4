#include "yocto/ios/icstream.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>

namespace yocto
{

	namespace ios
	{

		icstream:: ~icstream() throw() {}

		icstream:: icstream( const char   *filename ) : cfile( filename, "rb" ) {}
		icstream:: icstream( const string &filename ) : cfile( filename.c_str(), "rb" ) {}
		icstream:: icstream( const cstdin_t &x )      : cfile( x ) {}


		bool icstream:: query( char &C )
		{
			const int ans = fgetc( handle );
			if( EOF == ans )
			{
				if( feof(handle) )
					return false;
				throw libc::exception( EIO, "icstream::query" );
			}
			else
			{
				C = char(ans);
				return true;
			}

		}

		void icstream:: store( char C )
		{
			const int ans = C;
			if( ans != ungetc( C, handle) )
			{
				throw libc::exception( EIO, "icstream::store" );
			}
		}

	}

}
