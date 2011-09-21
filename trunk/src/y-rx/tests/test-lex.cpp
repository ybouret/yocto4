#include "yocto/utest/run.hpp"
#include "yocto/rx/lexer.hpp"
#include "yocto/rx/source.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(lex)
{
	std::cerr << "sizeof(regex::token)="  << sizeof(regex::token)  << std::endl;
	std::cerr << "sizeof(regex::lexeme)=" << sizeof(regex::lexeme) << std::endl;
	std::cerr << "sizeof(regex::lexical::rule)=" << sizeof(regex::lexical::rule) << std::endl;
	
	regex::lexer      L;
	regex::pattern_db dict;
	
	dict.record( "DBL", regex::compile("[:digit:]+.[:digit:]*",NULL) );
	
	L( "[:digit:]+", "INT" );
	L( "{DBL}",      "DBL", &dict );
	L( "{DBL}f",     "FLT", &dict );
 	L( "[:blank:]+", "WS"    );
	L( "[:word:]+",  "Word"  );
	L( "[:endl:]",   "ENDL"  );
	
	dict.release();
	
	regex::source src;
	ios::icstream inp( ios::cstdin );
	
	src.connect( inp );
	regex::lexemes lxs;
	for(;;)
	{
		regex::lexeme *lx = L.get( src );
		if( !lx ) break;
		lxs.push_back( lx );
		std::cerr << "<" << *lx << "> @" << lx->label << std::endl;
	}
	
	if( src.peek() )
		std::cerr << "Unknown char '" << src.peek()->data << "'" << std::endl;
	else {
		std::cerr << "EOS" << std::endl;
	}
	
	std::cerr << "recognized " << lxs.size << " lexemes" << std::endl;
	lxs.to(src);

	
	
}
YOCTO_UNIT_TEST_DONE()
