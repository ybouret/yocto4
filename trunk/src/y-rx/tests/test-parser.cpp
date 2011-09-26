#include "yocto/utest/run.hpp"
#include "yocto/rx/parser.hpp"
#include "yocto/rx/source.hpp"
#include "yocto/ios/icstream.hpp"


#include "yocto/rx/syntactic/terminal.hpp"

using namespace yocto;
using namespace regex;

YOCTO_UNIT_TEST_IMPL(parser)
{

	std::cerr << "sizeof(syntactic::rule)=" << sizeof(regex::syntactic::rule) << std::endl;

	ios::icstream     inp( ios::cstdin );
	source            src;
	
	src.connect( inp );
	lexer      lxr;

	lxr( "([:digit:]+.*)", "INT" );
	
	parser  prs( new syntactic::terminal( "INT" ) );
	prs.restart(lxr, src);
	
	src.connect( inp );
	const syntax::result res = prs( lxr, src, 0 );
	std::cerr << "result=" << res << std::endl;
	
}
YOCTO_UNIT_TEST_DONE()
