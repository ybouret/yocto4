//#include "yocto/ios/pstream.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/wtime.hpp"
#include "yocto/string.hpp"

#include <cstdio>
#include <cerrno>
#include <iostream>

using  namespace yocto;


int main( int argc, char *argv[] )
{
	int res = 0;
	
	try
	{
		if( argc > 1 )
		{
			string cmd = argv[1];
			for( int i=2; i < argc; ++i )
			{
				cmd += ' ' ;
				cmd += argv[i];				
			}
			std::cerr << "-- preparing to execute '" << cmd << "'" << std::endl;
			std::cerr.flush();
			FILE *fp = popen( cmd.c_str(), "r" );
			if( !fp )
				throw libc::exception( errno, "popen(%s)", cmd.c_str() );
			char C   = 0;
			wtime chrono;
			chrono.start();
			while( 1 == fread( &C, 1, 1, fp) )
			{
				fputc(C, stdout);
				fflush( stdout );
				if( chrono.query() > 5 )
				{
					std::cerr << "-- interrupted !" << std::endl;
					break;
				}
			}
			const int ans = pclose( fp );
			std::cerr.flush();
			std::cerr << "-- done [" <<  ans << "]" << std::endl;
		}
	}
	catch( const exception &e )
	{
		std::cerr << e.when() << std::endl;
		std::cerr << e.what() << std::endl;
	}
	catch(...)
	{
		std::cerr << "Unhandled Exception!" << std::endl;
		
		res = -1;
	}
	
	return res;
	
}