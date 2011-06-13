#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/exception.hpp"
#include "yocto/auto-ptr.hpp"

#include <iostream>

using namespace yocto;

int main( int argc, char *argv[] )
{
	try
	{
	
		
		//-- default: save has binary
		bool binary = true;
		
		//-- do we save as string ? ( add a zero )
		if( argc > 1 && !strcmp( "-s", argv[1]) )
		{
			binary = false;
			--argc;
			++argv;
		}
		
		//== input
		ios::icstream *input_ = NULL;
		if( argc > 1 )
		{
			const string filename = argv[1];
			input_ = new ios::icstream( filename );
		}
		else
		{
			input_ = new ios::icstream( ios::cstdin );
		}
		auto_ptr< ios::icstream > input( input_ );
		
		//== output
		ios::ocstream *output_ = NULL;
		if( argc > 2 )
		{
			const string filename = argv[2];
			output_ = new ios::ocstream( filename, false);
		}
		else
		{
			output_ = new ios::ocstream( ios::cstdout );
		}
		auto_ptr< ios::ocstream > output( output_ );
		
		size_t index = 0;
		size_t count = 0;
		char   c = 0;
		
		while( input->query(c) )
		{
			unsigned char C = c;
			if( count > 0 ) 
				output->write( ',' );
			if( index >= 16 )
			{
				output->write( '\n' );
				index = 0;
			}
			++index;
			++count;
			if( C >= 32 && C < 127 )
			{
				(*output)( "  '%c'", C );
			}
			else
			{
				(*output)( " 0x%02x", unsigned(C) );
			}
			
			
		}
		(*output)( " /* END */" );
		if( !binary )
		{
			if( count > 0 ) output->write(',');
			output->write('0');
		}
		output->write('\n');
		
		
		return 0;
	}
	catch( const exception &e )
	{
		std::cerr << e.when() << std::endl;
		std::cerr << e.what() << std::endl;
		return -1;
	}
	catch(...)
	{
		std::cerr << "unhandled exception!" << std::endl;
		return -1;
	}
	
}