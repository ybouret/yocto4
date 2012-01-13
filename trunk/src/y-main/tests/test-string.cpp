#include "yocto/utest/run.hpp"
#include "yocto/string.hpp"

using namespace yocto;

static inline void display( const string &s)
{
	std::cerr << "[" << s.size() << "/" << s.capacity() << "] '" << s << "'" << std::endl;
}

YOCTO_UNIT_TEST_IMPL(string)
{
	string a;            display(a);
	string b = 'a';      display(b);
	string c = "Hello";  display(c);
	string d = c;        display(d);
	
	b = 'b';     display(b);
	c = "world"; display(c);
	d = c;       display(d);
	b = "abcdefghijklmnopqrstuvwxyz1234567890"; display(b);
	b = &b[ b.size()/2 ]; display(b);
	b += c; b +=c; b += c; b += b; display(b);
	
	c = "Hello";
	d = " World";
	a = c + d; display(a);
	a = vformat( "now in '%s', line #%d, at '%s'", __FILE__, __LINE__, __DATE__ );
	display(a);
	
}
YOCTO_UNIT_TEST_DONE()


#include "yocto/string/conv.hpp"

YOCTO_UNIT_TEST_IMPL(strconv)
{

	for( int i=1; i < argc; ++i )
	{
	
		const string txt = argv[i];
		const string ctx = vformat( "argv[%d]", i );
		std::cerr << "'" << txt << "'" << std::endl;
		try
		{
			std::cerr << "to_real: " << strconv::to_real<double>( txt, ctx.c_str() ) << std::endl;
			std::cerr << "to_size: " << strconv::to_size( txt, ctx.c_str() ) << std::endl;
		}
		catch( const exception &e )
		{
			std::cerr << e.what() << std::endl;
			std::cerr << e.when() << std::endl;
		}
		catch(...)
		{
			throw;
		}
		
		
	}
	
}
YOCTO_UNIT_TEST_DONE()
