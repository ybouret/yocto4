#include "yocto/utest/run.hpp"
#include "yocto/rx/source.hpp"
#include "yocto/ios/icstream.hpp"


#include "yocto/rx/grammar.hpp"
#include "yocto/rx/syntax/terminal.hpp"
#include "yocto/rx/syntax/logical.hpp"

#include "yocto/ios/ocstream.hpp"

#include "yocto/functor.hpp"

#include <cstdlib>

using namespace yocto;
using namespace regex;

#define SHOW_SIZE(TYPE) std::cerr << "-- sizeof(" #TYPE ") = " << sizeof(TYPE) << std::endl

YOCTO_UNIT_TEST_IMPL(grammar)
{
	SHOW_SIZE(syntax::rule);
	SHOW_SIZE(syntax::terminal);
	SHOW_SIZE(syntax::counting);
	SHOW_SIZE(syntax::logical);
	std::cerr << std::endl;
	SHOW_SIZE(syntax::c_node);
	SHOW_SIZE(lexeme);
	std::cerr << std::endl;
	
	
	
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
	
	
	grammar G( "grammar" );
	{
		// registering root
		syntax::logical &root = G.aggregate( "list" );
		
		// registering terminals
		G.terminal( "INT"    );
		G.terminal( "RBRACK" , node_certain);
		G.terminal( "LBRACK" , node_certain);
		G.terminal( "COMMA"  , node_useless);
		G.terminal( "WORD"   );
		
		// create sub rules
		G.alternative( "item" ) << "INT" << "WORD" << "list";
		
		G.aggregate( "other", node_fusion ) << "COMMA" << "item";
		G.counting("optional","other", '*');
		
		// feed the root
		root << "LBRACK" << "item" << "optional" << "RBRACK";
		
	}
	
	syntax_result res = syntax_success;
	cst_node *node = G.parse(lxr, src, res);
	std::cerr << "res=" << int(res) << ", node@" << (void *)node << std::endl;
	if( node )
	{
		node->format(src.char_pool);
		{
			ios::ocstream fp( "parsed.dot", false );
			node->graphviz( "G", fp );
		}
		system( "dot -Tpng parsed.dot -o parsed.png" );
	}
	
	
}
YOCTO_UNIT_TEST_DONE()
