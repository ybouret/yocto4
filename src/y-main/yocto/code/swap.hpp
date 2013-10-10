#ifndef YOCTO_CODE_SWAP_INCLUDED
#define YOCTO_CODE_SWAP_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
    
	
	
#if 0
	template <typename T>
	inline void mswap( T &a, T &b ) throw()
	{
		uint8_t *p = (uint8_t *)&a;
		uint8_t *q = (uint8_t *)&b;
		for( size_t i=sizeof(T);i>0;--i,++p,++q)
			cswap( *p, *q );
	}
	
	template <typename T>
	inline void mmove( T &a, const T&b ) throw()
	{
		uint8_t *      p = (uint8_t *)&a;
		const uint8_t *q = (const uint8_t *)&b;
		for( size_t i=sizeof(T);i>0;--i,++p,++q)
			*p = *q;
	}
    
	
#endif
    
	
    
}


#endif
