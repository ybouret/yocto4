#ifndef YOCTO_HASH64_INCLUDED
#define YOCTO_HASH64_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
	
	namespace hashing
	{
		struct hash64
		{
			
			typedef void (*proc)( uint32_t *lword, uint32_t *rword );
			
			static void BJ( uint32_t *lword, uint32_t *rword ) throw(); /*!< Bob Jenkin's */
			static void NR( uint32_t *lword, uint32_t *rword ) throw(); /*!< NR's */
			
			static size_t   of( size_t i, hash64::proc h) throw();
			static uint64_t mix64(const uint64_t x, hash64::proc h) throw();
		};
		
	}
	
}
#endif
