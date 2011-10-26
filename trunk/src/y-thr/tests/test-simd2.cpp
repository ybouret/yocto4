#include "yocto/utest/run.hpp"
#include "yocto/threading/simd.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;
using namespace threading;

#include <cstdlib>

YOCTO_UNIT_TEST_IMPL(SIMD)
{
	size_t np = 2;
	size_t nc = 2;
	if( argc > 1 ) np = atol( argv[1] );
	if( argc > 2 ) nc = atol( argv[2] );
	std::cerr << ">>>> SIMD " << np << " " << nc << std::endl;
	
	std::cerr << "[Testing Alone]" << std::endl;
	{
		SIMD local_simd(np);
	}
	
	SIMD simd(np);
	ios::icstream in( ios::cstdin );
	string        line;
	
	while( line.clear(), fprintf(stderr,">"), fflush(stderr), in.read_line(line) >= 0 )
	{
		if( line == 'c' )
		{
			for( size_t i=0; i < nc; ++i )
			{
				//std::cerr << "[SIMD.enter cyle]" << std::endl;
				//simd.do_work = alea<double>() > 0.5;
				simd.cycle( & simd.idle );
				//std::cerr << "[SIMD.leave cyle]" << std::endl;
			}
			continue;
		}
		
		if( line == 'q' )
		{
			break;
		}
	}
	
	
}
YOCTO_UNIT_TEST_DONE()
