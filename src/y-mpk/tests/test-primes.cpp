#if 1
#include "yocto/utest/run.hpp"
#include "yocto/mpk/natural.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/sys/wtime.hpp"
#include "yocto/random/isaac.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/code/primes.hpp"
#include <cstdlib>

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(primes)
{
	wtime chrono;
	double ell = 0;
	mpn    p;
	size_t count = 0;
	
	size_t nmax  = argc > 1 ? static_cast<size_t>( strtol(argv[1], NULL, 10) ) : 1000;
	while( count < nmax )
	{
		++count;
		chrono.start();
		p = (++p).next_prime_();
		ell += chrono.query();
		if( p.bits() <= 8*sizeof(size_t) )
		{
			const size_t q = p.to<size_t>();
			if( ! is_prime(q) )
			{
				throw exception("Not a Prime !!");
			}
		}
		std::cerr << "#" << count << ": " << p << std::endl;
	}
	std::cerr << "Time=" << ell << std::endl;
	
	
}
YOCTO_UNIT_TEST_DONE()
#endif


