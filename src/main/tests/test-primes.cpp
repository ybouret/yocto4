#include "yocto/utest/run.hpp"
#include "yocto/code/primes.hpp"
#include "yocto/code/rand.hpp"

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
	
	for( size_t i=0; i < 10; ++i )
	{
		for( size_t j=0; j < 4; ++j )
		{
			const size_t n = 2 + alea_lt( 0xFFFFFF );
			std::cerr << "(" << prev_prime(n) << "," << n << "," << next_prime(n) << ") ";
		}
		std::cerr << std::endl;
	}
}
YOCTO_UNIT_TEST_DONE()


#include "yocto/code/primes-diff.hpp"
YOCTO_UNIT_TEST_IMPL(Primes)
{
    core::Primes::Print();
    for(size_t i=0;i<=20;++i)
    {
        std::cerr << i << " => " << nextPrime(i) << std::endl;
    }
    
}
YOCTO_UNIT_TEST_DONE()

