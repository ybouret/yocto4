#ifndef YOCTO_CODE_SWAP_INCLUDED
#define YOCTO_CODE_SWAP_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto 
{
	template <typename T>
	inline void cswap( T &a, T &b ) throw()
	{
		const T tmp(a);
		a = b;
		b = tmp;
	}
	
	template <typename T>
	inline void cswap_const( const T &a, const T &b ) throw()
	{
		cswap<T>( (T&)a, (T&)b );
	}
	
	
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
	
	inline void memswap( void *target, void *source, size_t length ) throw()
	{
		assert( !(NULL==target && length > 0 ) );
		assert( !(NULL==source && length > 0 ) );
		uint8_t *p = (uint8_t *)&target;
		uint8_t *q = (uint8_t *)&source;
		for( size_t i=length;i>0;--i,++p,++q) cswap( *p, *q );
	}
	
}


#endif
