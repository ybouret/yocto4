#include "yocto/utest/run.hpp"
#include "yocto/rx/lexer.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(lex)
{
	std::cerr << "sizeof(regex::token)="  << sizeof(regex::token)  << std::endl;
	std::cerr << "sizeof(regex::lexeme)=" << sizeof(regex::lexeme) << std::endl;
	std::cerr << "sizeof(regex::lexical::rule)=" << sizeof(regex::lexical::rule) << std::endl;
	
	regex::lexer L;
	L( "[:digit:]+", "Digit" );
	L( "[:blank:]+", "WS"    );
	
	
}
YOCTO_UNIT_TEST_DONE()
