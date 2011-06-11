#include "yocto/utest/run.hpp"
#include "yocto/code/primes.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(primes)
{
	size_t p=0;
	for( size_t i=0; i < 25; ++i )
	{
		for( size_t j=0; j < 16; ++j, ++p )
		{
			std::cerr << (p=next_prime(p)) << " ";
		}
		std::cerr << std::endl;
	}
}
YOCTO_UNIT_TEST_DONE()



