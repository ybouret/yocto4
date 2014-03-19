#include "yocto/mpa/natural.hpp"
#include "yocto/code/primes-diff.hpp"
#include <iostream>

namespace yocto
{
    namespace mpa
    {
        
#if 0
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
#endif
        
        bool natural:: is_prime_() const
        {
            const unsigned b0 = byte[0];
            switch(size)
            {
                case 0:
                    return false;
                case 1:
                    switch(b0)
                {
                    case 1:
                        return false;
                    case 2:
                    case 3:
                        return true;
                    default:
                        break;
                }
                default:
                    break;
            }
            
            
            assert(size>0);
            if( !(b0&1) )
            {
                //__________________________________________________________
                //
                // even
                //__________________________________________________________
                return false;
            }
            else
            {
                //__________________________________________________________
                //
                // div by 3 ?
                //__________________________________________________________
                mpn q; q.size =1 ; q.byte[0] = 3;
                if( is_divisible_by(q) )
                {
                    return false;
                }
                else
                {
                    //______________________________________________________
                    //
                    // use sieve >= 5
                    //______________________________________________________
                    size_t w = 3;
                    mpn    q2;
                    for(size_t i=0; i < core::Primes::DIFF_SIZE;++i)
                    {
                        w += core::Primes::DIFF_BYTE[i];
                        const uint64_t W(w);
                        const uint64_t W2 = W*W;
                        q2 = W2;
                        if( compare(q2,*this) > 0 )
                            return true;
                        q = W;
                        if( is_divisible_by(q) )
                            return false;
                    }
                    
                    //______________________________________________________
                    //
                    // generic case
                    //______________________________________________________
                    q = uint64_t(core::Primes::NEXT);
                    while(true)
                    {
                        q2 = sqr(q);
                        if( compare( q2, *this) > 0 )
                            return true;
                        if( is_divisible_by(q) )
                            return false;
                        q.inc(2);
                    }
                    
                }
                
            }
        }
        
        
        natural natural:: next_prime_() const
        {
            const unsigned b0 = byte[0];
            if(size<=0|| (size==1 && (b0==1||b0==2)) )
                return two();
            else
            {
                mpn q = *this;
                if(q.is_even()) ++q;
                while( ! q.is_prime_() )
                {
                    q.inc(2);
                }
                return q;
            }
        
        }
        
    }
    
}
