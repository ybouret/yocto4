#include "yocto/mpk/natural.hpp"
#include "yocto/code/primes16.hpp"

//#include <iostream>

namespace yocto
{
	namespace mpk
	{
		
		bool natural:: is_prime_() const
		{
			static const uint64_t p32 = YOCTO_PRIMES16_NEXT;
			if( size_ <= 1 && byte_[0] <= 1 )
			{
				return false;
			}
			else
			{
				//--------------------------------------------------------------
				//-- precomputed sieve
				//--------------------------------------------------------------
				{
					for( size_t i=0; i < YOCTO_PRIMES16_COUNT; ++i )
					{
						const uint16_t w  = core::primes16::table[i]; assert( w > 0 );
						const natural  q  = w;                        assert( q.is_positive() );
						const natural  q2 = sqr_(q);
						
						if( compare( q2, *this) > 0 ) 
							return true;
						const natural m  = mod_( *this, q );
						
						if( m.is_zero() )
							return false;
					}
				}
				
				//--------------------------------------------------------------
				//-- then go on
				//--------------------------------------------------------------
				{
					natural q = p32;
					while( true )
					{
						const natural q2 = sqr_(q);
						if( compare( q2, *this) > 0 ) 
							return true;
						assert( q.is_positive() );
						const natural m = mod_( *this, q );
						if( m.is_zero() )
							return false;
						natural tmp = q.inc_(2);
						q.xch( tmp );
					}				
					//return true;
				}
			}
			
		}
		
		natural natural:: next_prime_() const
		{
			natural q = *this;
			if( q.is_even() ) q.inc(1); assert( q.is_odd() );
			while( ! q.is_prime_() ) 
			{
				natural tmp = q.inc_(2);
				q.xch( tmp );
			}
			return q;
		}
		
	}
	
}
