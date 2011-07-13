#include "yocto/utest/run.hpp"
#include "yocto/ios/raw-file.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(raw_file)
{
	size_t       done = 0;
	const string msg = "Hello, World!\n";
	
	{
		ios::raw_file fp( "foo.raw", ios::writable | ios::truncate );
		fp.put( msg.ro(), msg.length(), done);
	}
	
	{
		ios::raw_file fp( ios::cstdout );
		fp.put( msg.ro(), msg.length(), done);
	}
	
	{
		ios::raw_file fp( "foo.raw", ios::readable | ios::writable );
		char buffer[8];
		fp.get( buffer, 5, done );
		const string str( buffer, done );
		std::cerr << "<" << str << ">" << std::endl;
		buffer[0] = '&';
		buffer[1] = '&';
		fp.put( buffer, 2, done );
	}
	
}
YOCTO_UNIT_TEST_DONE()
