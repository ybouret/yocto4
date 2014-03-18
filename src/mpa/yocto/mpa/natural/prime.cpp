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
        
        static const uint8_t p24[16384] =
        {
#include "p24.cxx"
        };
        
        static const size_t   N1  = 53;
        static const size_t   N2  = 6488;
        static const size_t   N3  = 1118;
        static const uint64_t NXT = 77983;
        
        bool natural:: is_prime24() const
        {
            if( size <= 1 && byte[0] <= 1 )
			{
				return false;
			}
			else
			{
                size_t idx = 0;
                for(size_t i=N1;i>0;--i)
                {
                    const uint64_t w  = p24[idx++];
                    const natural  q  = w;
                    const natural  q2 = w*w;
                    
                    if( compare( q2, *this) > 0 )
                        return true;
                    
                    //std::cerr << "testing " << q << std::endl;
                    if( is_divisible_by(q) )
                        return false;
                }
                
                for(size_t i=N2;i>0;--i)
                {
                    const uint64_t b0 = p24[idx++];
                    const uint64_t b1 = p24[idx++];
                    const uint64_t w  = b0 | (b1<<8);
                    const natural  q  = w;
                    const natural  q2 = w*w;
                    
                    if( compare( q2, *this) > 0 )
                        return true;
                    //std::cerr << "testing " << q << std::endl;
                    if( is_divisible_by(q) )
                        return false;
                }

                for(size_t i=N3;i>0;--i)
                {
                    const uint64_t b0 = p24[idx++];
                    const uint64_t b1 = p24[idx++];
                    const uint64_t b2 = p24[idx++];
                    const uint64_t w  = b0 | (b1<<8) | (b2<<16);
                    const natural  q  = w;
                    const natural  q2 = w*w;
                    
                    if( compare( q2, *this) > 0 )
                        return true;
                    
                    //std::cerr << "testing " << q << std::endl;
                    if( is_divisible_by(q) )
                        return false;
                }
                
                {
					natural q = NXT;
					while( true )
					{
						const natural q2 = sqr(q);
						if( compare( q2, *this) > 0 )
							return true;
                        std::cerr << "testing " << q << std::endl;
						if( is_divisible_by(q) )
                            return false;
						q.inc(2);
					}
				}
                
                
            }
            
        }
        
    }

}
