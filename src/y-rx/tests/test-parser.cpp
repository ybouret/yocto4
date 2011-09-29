#include "yocto/utest/run.hpp"
#include "yocto/rx/parser.hpp"
#include "yocto/rx/source.hpp"
#include "yocto/ios/icstream.hpp"


#include "yocto/rx/syntactic/terminal.hpp"
#include "yocto/rx/syntactic/logical.hpp"
#include "yocto/rx/syntactic/counting.hpp"

#include "yocto/ios/ocstream.hpp"

#include <cstdlib>

using namespace yocto;
using namespace regex;

YOCTO_UNIT_TEST_IMPL(parser)
{
	
	std::cerr << "sizeof(syntactic::rule)  =" << sizeof(regex::syntactic::rule)   << std::endl;
	std::cerr << "sizeof(syntactic::s_node)=" << sizeof(regex::syntactic::s_node) << std::endl;
	
	ios::icstream     inp( ios::cstdin );
	source            src;
	
	src.connect( inp );
	lexer      lxr;
	
	lxr( "\\[",         "LBRACK" );
	lxr( "\\]",         "RBRACK" );
	lxr( "[:digit:]+",  "INT" );
	lxr( "[ \\t]",      "WS"  );
	
	auto_ptr<s_logical> p( s_and::create( "list" ) );
	*p <<   s_terminal::create( "LBRACK" );
	
	*p <<   s_terminal::create( "RBRACK" );
	
	{
		auto_ptr<s_logical> q( s_and::create( "sub_expr" ) );
		*q <<  s_terminal::create( "COMMA" );
		*q <<  s_optional::create( "BLANKS", s_terminal::create("WS") );
		*q <<  s_terminal::create( "INT"   );
		*p <<  s_optional::create( "following", q.yield() );
	}
	
	
	parser prs( p.yield() );
	prs.restart(lxr, src);
	src.connect( inp );
	
	const syntax::result res = prs( lxr, src );
	std::cerr << "result=" << res << std::endl;
	if( src.is_active() )
		std::cerr << "but source didn't end !!!" << std::endl;
	
	{
		ios::ocstream fp( "parsed.dot", false );
		prs.graphviz( fp, "G" );
	}
	system("dot -Tpng parsed.dot -o parsed.png");
}
YOCTO_UNIT_TEST_DONE()
