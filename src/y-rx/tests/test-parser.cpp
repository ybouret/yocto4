#include "yocto/utest/run.hpp"
#include "yocto/rx/parser.hpp"
#include "yocto/rx/source.hpp"
#include "yocto/ios/icstream.hpp"


#include "yocto/rx/syntactic/terminal.hpp"
#include "yocto/rx/syntactic/logical.hpp"
#include "yocto/rx/syntactic/counting.hpp"

#include "yocto/ios/ocstream.hpp"

#include "yocto/functor.hpp"

#include <cstdlib>

using namespace yocto;
using namespace regex;

YOCTO_UNIT_TEST_IMPL(parser)
{
	
	typedef functor<bool,TL1(regex::token&)> cb_t;
	std::cerr << "sizeof(cb_t)=" << sizeof(cb_t) << std::endl;
	std::cerr << "sizeof(syntactic::rule)  =" << sizeof(regex::syntactic::rule)   << std::endl;
	std::cerr << "sizeof(syntactic::s_node)=" << sizeof(regex::syntactic::s_node) << std::endl;
	
	ios::icstream     inp( ios::cstdin );
	source            src;
	
	src.connect( inp );
	lexer      lxr;
	
	lxr( "\\[",         "LBRACK" );
	lxr( "\\]",         "RBRACK" );
	lxr( ",",           "COMMA"  );
	lxr( "[:digit:]+",  "INT" );
	lxr( "[:word:]+",   "WORD" );
	lxr( "[ \\t]+",     "WS",   lxr, & lexer::skip );
	lxr( "[:endl:]",    "ENDL", lxr, & lexer::skip );
	
	auto_ptr<s_logical> p( s_and::create( "list" ) );
	*p <<   s_symbol::create( "LBRACK" );
	{
		auto_ptr<s_logical> item( s_or::create("item") );
		*item << s_terminal::create( "INT" );
		*item << s_terminal::create( "WORD" );
		*p << item->clone();
		
		auto_ptr<s_logical> q( s_and::create("optional_item") );
		*q << s_symbol::create("COMMA") << item->clone();
		*p << s_any_count::create("any_other", q.yield());
	}
	*p <<   s_symbol::create( "RBRACK" );
	
	
	
	
	//parser prs( s_any_count::create( "grammar", p.yield() ) );
	parser prs( p.yield() );
	prs.restart(lxr, src);
	src.connect( inp );
	
	const syntax::result res = prs( lxr, src );
	std::cerr << "result=" << syntax::get_info(res) << std::endl;
	if( src.is_active() )
		std::cerr << "but source didn't end !!!" << std::endl;
	std::cerr << "lxr.cache.size=" << lxr.cache.size << std::endl;
	{
		ios::ocstream fp( "parsed.dot", false );
		prs.graphviz( fp, "G" );
	}
	system("dot -Tpng parsed.dot -o parsed.png");
	
	
}
YOCTO_UNIT_TEST_DONE()
