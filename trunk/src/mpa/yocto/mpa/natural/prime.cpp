#include "yocto/mpa/natural.hpp"
#include "yocto/code/primes16.hpp"
#include <iostream>

namespace yocto
{
    namespace mpa
    {
        
        bool natural:: is_prime16() const
        {
            
            static const uint64_t p32 = YOCTO_PRIMES16_NEXT;
			if( size <= 1 && byte[0] <= 1 )
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
						const uint64_t w  = core::primes16::table[i]; assert( w > 0 );
						const natural  q  = w;
						const natural  q2 = w*w;
						
						if( compare( q2, *this) > 0 )
							return true;
                        if( is_divisible_by(q) )
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
						const natural q2 = sqr(q);
						if( compare( q2, *this) > 0 )
							return true;
						if( is_divisible_by(q) )
                            return false;
						q.inc(2);
					}
				}
			}

        }
        
               
    }

}
