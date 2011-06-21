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
			if( 1 != fread( &C, 1, 1, handle) )
			{
				if( feof(handle) )
					return false;
				throw libc::exception( EIO, "icstream::query()" );
			}
			else {
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
		
		size_t icstream:: get( void *buffer, size_t buflen )
		{
			const size_t ans = fread( buffer, 1, buflen, handle );
			if( ferror(handle) )
				throw libc::exception( EIO, "icstream::get(%u)", unsigned(buflen) );
			return ans;
		}
		
		
		bool icstream:: gets( string &line )
		{
			YOCTO_LOCK(*this);
			line.clear();
			return read_line( line ) >= 0;
		}
	}
	
}

