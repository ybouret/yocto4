#ifndef YOCTO_CODE_PRIMES16_INCLUDED
#define YOCTO_CODE_PRIMES16_INCLUDED 1

#include "yocto/os.hpp"

/*! number of primes with 16 bits */
#define YOCTO_PRIMES16_COUNT ( 6542  )

/*! first prime with 17 bits */
#define YOCTO_PRIMES16_NEXT  ( 65537 )

namespace yocto
{
	namespace core
	{
		struct primes16
		{
			static const uint16_t table[ YOCTO_PRIMES16_COUNT ];
		};
		
	}
}
#endif
