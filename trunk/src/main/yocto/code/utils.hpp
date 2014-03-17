#ifndef YOCTO_UTILS_INCLUDED
#define YOCTO_UTILS_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
	
	template <typename T>
	inline T min_of( T a, T b )
	{
		return a < b ? a : b;
	}
	
	template <typename T>
	inline T max_of( T a, T b )
	{
		return b < a ? a : b;
	}
	
    template <typename T>
    inline int sign_of( T a )
    {
        return a < 0 ? -1 : (  0 < a ? 1 : 0 );
    }
       
	template <typename T>
	inline T clamp( T amin, T a, T amax )
	{
		return a < amin ? amin : ( amax < a ? amax : a );
	}
	
	//! integer hash
	uint32_t ihash32( uint32_t );
	
	extern const char  hexa_char[16];
    extern const char *hexa_text[256];
	int                hex2dec( char c ) throw();
	
	template <typename T>
	inline bool is_a_power_of_two( T v ) throw()
	{
		static const T one(1);
		return (v != 0) && !( v & (v - one) );
	}
	
	template <typename T>
	inline T next_power_of_two( T v ) throw()
	{
		assert(v>0);
        --v;
        for( size_t shift = 1; shift <= (sizeof(T)<<2); shift <<= 1 )
            v |= ( v >> shift );
        return ++v;
	}
    
    
    
	
	template <typename T>
	inline T gcd_of( T x , T y )
	{
		if( x > 0 && y > 0 )
		{
			T a = x;
			T b = y;
			if( b > a )
			{
				a = y;
				b = x;
			}
			while( b > 0 )
			{
				const T t = b;
				b = a % b;
				a = t;
			}
			return a;
		}
		else
			return 1;
	}
	
	template <typename T>
	inline T lcm_of( T x , T y )
	{
		return ( x * y ) / gcd_of<T>( x, y );
	}
	
}

#endif
