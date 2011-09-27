#include "yocto/utest/run.hpp"
#include "yocto/rx/parser.hpp"
#include "yocto/rx/source.hpp"
#include "yocto/ios/icstream.hpp"


#include "yocto/rx/syntactic/terminal.hpp"
#include "yocto/rx/syntactic/logical.hpp"
#include "yocto/rx/syntactic/counting.hpp"

using namespace yocto;
using namespace regex;

YOCTO_UNIT_TEST_IMPL(parser)
{

	std::cerr << "sizeof(syntactic::rule)=" << sizeof(regex::syntactic::rule) << std::endl;

	ios::icstream     inp( ios::cstdin );
	source            src;
	
	src.connect( inp );
	lexer      lxr;

	lxr( "[:digit:]+", "INT" );
	lxr( "[ \\t\\r\\n]",   "WS"  );
	
	/*
	auto_ptr<s_logical> q( s_and::create( "expr" ) );
	*q <<  s_terminal::create( "INT" );
	*q << s_optional::create( "BLANKS", s_terminal::create("WS") );
	*/
	//parser  prs( s_one_ore_more::create("list", q.yield()) );
	parser prs( s_terminal::create( "INT" ) );
	prs.restart(lxr, src);
	
	src.connect( inp );
	const syntax::result res = prs( lxr, src );
	std::cerr << "result=" << res << std::endl;
	if( src.is_active() )
		std::cerr << "but source didn't end !!!" << std::endl;
}
YOCTO_UNIT_TEST_DONE()
