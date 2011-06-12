#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/auto-ptr.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(streams)
{
	auto_ptr<ios::istream>   input;
	auto_ptr<ios::ocstream>  output;
	
	if( argc > 1 )
		input = new ios::icstream( argv[1] );
	else 
		input = new ios::icstream( ios::cstdin );

	
	if( argc > 2 )
		output = new ios::ocstream( argv[2], false );
	else 
		output = new ios::ocstream( ios::cstdout );
	

	
	char C;
	while( input->query(C) )
	{
		output->write(C);
	}
	
}
YOCTO_UNIT_TEST_DONE()


