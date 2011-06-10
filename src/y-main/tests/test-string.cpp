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
	
}
YOCTO_UNIT_TEST_DONE()

