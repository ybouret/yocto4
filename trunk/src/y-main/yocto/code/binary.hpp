//! \file binary.hpp
/**
	\brief binary utilities.
 */
#ifndef YOCTO_BINARY_INCLUDED
#define YOCTO_BINARY_INCLUDED 1

#include "yocto/type-ints.hpp"


namespace yocto
{


	size_t bytes_for( const size_t n ) throw();
	size_t bits_for( const size_t n ) throw();

	extern const uint8_t ceil_ln2_table[256];




	template <typename T>
	inline size_t ceilLn2( const T &x ) throw()
	{
		const typename unsigned_int< sizeof(T) >::type I(x);
		for( register size_t i = sizeof(T); i >0; )
		{

			const size_t nsh = (--i) << 3;
			const size_t ans = ceil_ln2_table[ ( I >> nsh ) & 0xFF ];
			if( ans > 0 )
				return ans + nsh;
		}
		return ceil_ln2_table[ I & 0xFF ];
	}




}

#endif
