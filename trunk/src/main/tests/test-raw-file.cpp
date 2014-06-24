#include "yocto/utest/run.hpp"
#include "yocto/ios/raw-file.hpp"
#include "yocto/ios/irchannel.hpp"
#include "yocto/ios/orchannel.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(raw_file)
{
	size_t       done = 0;
	const string msg = "Hello, World!\n";
	char         buffer[8];
	
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
		
		done = fp.get( buffer, 5);
		const string str( buffer, done );
		std::cerr << "<" << str << ">" << std::endl;
		buffer[0] = '&';
		buffer[1] = '&';
		fp.put( buffer, 2, done );
		fp.rewind();
		buffer[0]='h';
		fp.put( buffer, 1, done );
	}
	
	{
		ios::irchannel in( "foo.raw", 0 );
        scoped_lock guard( in.handle() );
        
		do {
			done = in.get( buffer, sizeof(buffer)  );
			std::cerr.write( buffer, done );
		} while( done > 0 );
	}
    
    {
        ios::orchannel fp("bar.raw");
        fp.put(msg.ro(), msg.length(), done);
    }
	
	
}
YOCTO_UNIT_TEST_DONE()
