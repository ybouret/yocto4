#include "yocto/code/primes.hpp"
#include "yocto/code/primes16.hpp"

#include "yocto/exceptions.hpp"
#include <cerrno>

namespace yocto
{
	
	static inline bool _is_prime_above2( size_t n ) throw()
	{
		size_t q;
		assert( n >= 2 );
		for( size_t i=0; i < YOCTO_PRIMES16_COUNT; ++i )
		{
			q = core::primes16::table[i];
			if( (q * q) >  n ) return true;
			if( (n % q) == 0 ) return false;
		}
		q = YOCTO_PRIMES16_NEXT;
		while( q * q <= n )
		{
			if( (n % q) == 0 ) return false;
			q += 2;
		}
		return true;
	}
	
	bool is_prime( size_t n ) throw()
	{
		if( n < 2 )
			return false;
		else
		{
			return _is_prime_above2(n);
		}
	}
	
	size_t next_prime( size_t n )
	{
		if( n <= 2 )
		{
			return 2;
		}
		else 
		{
			size_t q = n;
			if( ! (q&1) ) ++q;
			while( ! _is_prime_above2( q ) )
			{
				q += 2;
			}
			if( q < n )
				throw libc::exception( ERANGE, "next_prime(%u) overflow", unsigned(n) );
			return q;
		}
		
	}
	
	
}