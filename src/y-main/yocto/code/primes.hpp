#ifndef YOCTO_CODE_PRIMES_INCLUDED
#define YOCTO_CODE_PRIMES_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
	bool   is_prime( size_t n ) throw();
	size_t next_prime( size_t n );
	size_t prev_prime( size_t n );
	
}


#endif
