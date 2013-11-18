#include "yocto/hashing/fnv.hpp"

namespace yocto
{
	namespace hashing
	{
		
		fnv:: fnv() throw() : function( __length, __window ), hash(0)
		{
		}
		
		fnv:: ~fnv() throw()
		{
			hash = 0;
		}
		
		const char * fnv:: name() const throw() { return "FNV"; }
		
		void fnv:: set() throw()
		{
			hash = 0;
		}
		
		void fnv:: run( const void *buffer, size_t buflen ) throw()
		{
			assert( !(buffer==NULL&&buflen>0) );
			static const uint32_t fnv_prime = 0x811C9DC5;
			const uint8_t *str = (const uint8_t *)buffer;
			
			for(;buflen>0; --buflen, ++str)
			{
				hash *= fnv_prime;
				hash ^= (*str);
			}
			
		}
		
		void fnv:: get( void *output, size_t outlen ) throw()
		{
			fill(output, outlen, &hash, sizeof(hash) );
		}
		
		
	}
	
}

