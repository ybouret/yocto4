#include "yocto/utest/run.hpp"
#include "yocto/rx/pattern/posix.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/auto-ptr.hpp"
#include "yocto/rx/pattern/db.hpp"

#include <cstdlib>

using namespace yocto;


static inline void test_posix( const string &name, const regex::pattern &p )
{
	const string fdot = name + ".dot";
	const string fpng = name + ".png";
	
	{
		ios::ocstream fp( fdot, false );
		p.graphviz( fp, "G" );
	}
	
	const string cmd = "dot -Tpng " + fdot + " -o " + fpng;
	std::cerr << cmd << std::endl;
	system( cmd.c_str() );
	
}

#define _TEST(NAME) \
do { auto_ptr<regex::pattern> p( regex::posix::NAME() ); \
 test_posix( #NAME, *p); \
 db.record( #NAME, p.yield() ); \
 p.reset( db.create( #NAME ) ); } while( false )

YOCTO_UNIT_TEST_IMPL(posix)
{
	regex::pattern_db db;
	_TEST(lower);
	_TEST(upper);
	_TEST(digit);
	_TEST(alpha);
	_TEST(alnum);
	_TEST(xdigit);
	_TEST(blank);
	_TEST(space);
	_TEST(punct);
	_TEST(word);
	_TEST(dot);
	_TEST(endl);
	_TEST(cstring);
}
YOCTO_UNIT_TEST_DONE()
