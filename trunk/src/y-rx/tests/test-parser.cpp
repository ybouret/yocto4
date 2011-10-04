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

YOCTO_UNIT_TEST_IMPL(parser)
{
	std::cerr << "sizeof(syntax::rule)=" << sizeof(syntax::rule) << std::endl;
	
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
		syntax::logical &root = G.agg( "list" );
		
		// registering terminals
		G.term( "INT" );
		G.term( "RBRACK" );
		G.term( "LBRACK" );
		G.term( "COMMA"  );
		
		// create sub rules
		 G.alt( "item" ) << "INT" << "list";
		
		G.agg( "other" ) << "COMMA" << "item";
		G.counting("optional", "other", '*');
		
		// feed the root
		root << "LBRACK" << "item" << "optional" << "RBRACK";
		
	}
	
	
	const syntax_result res = G.parse(lxr, src);
	std::cerr << "res=" << int(res) << std::endl;
	
}
YOCTO_UNIT_TEST_DONE()
