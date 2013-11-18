#include "yocto/os.hpp"

namespace yocto
{
	const as_capacity_t as_capacity = {};
	
	const char *platform_name() throw()
	{
		return YOCTO_PLATFORM;
	}
	
#if !defined (get16bits)
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
+(uint32_t)(((const uint8_t *)(d))[0]) )
#endif
	
	uint32_t hash32( const void *buffer, size_t length ) throw()
	{
		const uint8_t *data = (const uint8_t *)buffer;
		
		assert( !(buffer==NULL && length>0) );
		
		if (length<=0)
		{
			return 0;
		}
		else
		{
			uint32_t       hash = length;
			const uint32_t rem  = length & 3;
			uint32_t       tmp  = 0;
			
			/* Main loop */
			for ( length >>= 2 ;length > 0; length--)
			{
				hash  += get16bits (data);
				tmp    = (get16bits (data+2) << 11) ^ hash;
				hash   = (hash << 16) ^ tmp;
				data  += 2*sizeof (uint16_t);
				hash  += hash >> 11;
			}
			
			/* Handle end cases */
			switch (rem) 
			{
				case 3: hash += get16bits (data);
					hash ^= hash << 16;
					hash ^= data[sizeof (uint16_t)] << 18;
					hash += hash >> 11;
					break;
				case 2: hash += get16bits (data);
					hash ^= hash << 11;
					hash += hash >> 17;
					break;
				case 1: hash += *data;
					hash ^= hash << 10;
					hash += hash >> 1;
			}
			
			/* Force "avalanching" of final 127 bits */
			hash ^= hash << 3;
			hash += hash >> 5;
			hash ^= hash << 4;
			hash += hash >> 17;
			hash ^= hash << 25;
			hash += hash >> 6;
			
			return hash;
		}
		
	}
}

