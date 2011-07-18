#include "yocto/utest/run.hpp"
#include "yocto/rx/source.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(source)
{
	ios::icstream input( ios::cstdin );
	regex::source src;
	
	src.connect( input );
	src.prefetch( 1 + alea_less_than<size_t>(16) );
	regex::t_char *ch = NULL;
	while( NULL != ( ch = src.get() ) )
	{
		std::cerr << ch->data;
		src.skip(ch);
		if( src.in_cache() <= 0 )
			src.prefetch( 1 + alea_less_than<size_t>(16) );
	}
	
}
YOCTO_UNIT_TEST_DONE()
