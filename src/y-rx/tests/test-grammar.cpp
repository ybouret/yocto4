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
	SHOW_SIZE(grammar);
	
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
	
	syntax::c_node *node = G.parse(lxr, src );
	assert(node);
	//node->format(G,src.char_pool);
	{
		ios::ocstream fp( "parsed.dot", false );
		node->graphviz( "G", fp );
	}
	system( "dot -Tpng parsed.dot -o parsed.png" );
	
	
	
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/rx/pattern/basic.hpp"

YOCTO_UNIT_TEST_IMPL(expr)
{
	lexer lxr;
	lxr( "[:digit:]+", "INT" );
	lxr( "\\(", "LPAREN" );
	lxr( "\\)", "RPAREN" );
	lxr( "[*/]", "F_OP");
	lxr( "[-+]", "S_OP");
	lxr( "[ \\t]+",     "WS",   lxr, & lexer::skip );
	lxr( "[:endl:]",    "ENDL", lxr, & lexer::skip );
	
	grammar G( "expression" );
	
	{
		syntax::logical &expr   = G.aggregate( "expr" );
		syntax::logical &atom   = G.alternative( "atom" );
		syntax::logical &factor = G.aggregate("factor");
		
		G.terminal( "INT" );
		G.terminal( "LPAREN" , node_useless);
		G.terminal( "RPAREN" , node_useless);
		G.terminal( "F_OP" );
		G.terminal( "S_OP" );
		
		G.aggregate("sub_atom") << "LPAREN" << "expr" << "RPAREN";
		atom << "INT" << "sub_atom";
		
		G.aggregate( "sub_factor" ) << "F_OP" << "atom";
		G.counting(  "factor_rhs", "sub_factor", '*');
		factor << "atom" << "factor_rhs";
		
		G.aggregate( "sub_expr" ) << "S_OP" << "factor";
		G.counting( "expr_rhs", "sub_expr", '*' );
		expr << "factor" << "expr_rhs";
		
		
	}
	
	ios::icstream     inp( ios::cstdin );
	source            src;
	
	src.connect( inp );
	syntax::c_node *node = G.parse(lxr, src);
	assert(node);
	{
		ios::ocstream fp( "expr.dot", false );
		node->graphviz( "G", fp );
	}
	system( "dot -Tpng expr.dot -o expr.png" );
	
	
}
YOCTO_UNIT_TEST_DONE()

YOCTO_UNIT_TEST_IMPL(json)
{
	lexer   L;
	grammar G("json");
	
	L( "\\{", "LBRACE" );
	L( "\\}", "RBRACE" );
	L( "[:cstring:]", "string" );
	L( "[:digit:]+",  "number" );
	L( ",", "COMMA" );
	L( ":", "COLUMN");
	L( "[ \\t]+",     "WS",   L, & lexer::skip );
	L( "[:endl:]",    "ENDL", L, & lexer::skip );
	{
		syntax::logical &json = G.alternative( "json" );
		syntax::logical &obj  = G.alternative( "object" );
		
		G.terminal( "LBRACE" );
		G.terminal( "RBRACE" );
		G.terminal( "string" );
		G.terminal( "number" );
		G.terminal( "COMMA" , node_certain );
		G.terminal( "COLUMN", node_certain );
		G.alternative( "value" ) << "string" << "number" << "object";
		G.aggregate("pair") << "string" << "COLUMN" << "value";
		syntax::logical &members = G.aggregate( "members" );
		G.aggregate( "other_members" ) << "COMMA" << "members";
		G.counting( "optional_members", "other_members", '*');
		members << "pair" << "optional_members";

		
		G.aggregate("empty_object") << "LBRACE" << "RBRACE";
		G.aggregate("heavy_object") << "LBRACE" << "members" << "RBRACE";
		
		obj << "empty_object" << "heavy_object";
		
		json << "object";
		
	}
	
	ios::icstream     inp( ios::cstdin );
	source            src;
	
	src.connect( inp );
	syntax::c_node *node = G.parse(L, src);
	assert(node);
	{
		ios::ocstream fp( "json.dot", false );
		node->graphviz( "G", fp );
	}
	system( "dot -Tpng json.dot -o json.png" );
	
	
}
YOCTO_UNIT_TEST_DONE()


