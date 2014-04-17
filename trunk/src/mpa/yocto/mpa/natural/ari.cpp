#include "yocto/mpa/natural.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>

namespace yocto
{
    namespace mpa
    {
        natural natural::gcd( const natural &x, const natural &y )
		{
			if( x.size > 0 && y.size > 0 )
			{
				
				natural a( x );
				natural b( y );
				while( b.size > 0 )
				{
					natural r( __mod( a, b ) );
					a.xch(b);
					b.xch(r);
				}
				return a;
			}
			else
			{
				return natural::one();
			}
		}
		
        void natural:: simplify( natural &lhs, natural &rhs )
        {
            const natural d = gcd(lhs,rhs);
            natural       L = lhs/d;
            natural       R = rhs/d;
            lhs.xch(L);
            rhs.xch(R);
        }
        
        natural natural:: factorial( uint64_t n )
        {
            natural ans = one();
            while(n>0)
            {
                ans *= n;
                --n;
            }
            return ans;
        }
        
        
		bool natural::are_coprime( const natural &x, const natural &y )
		{
			if( x.size > 0 && y.size > 0 )
			{
				
				natural a( x );
				natural b( y );
				while( b.size > 0 )
				{
					natural r( __mod( a, b ) );
					a.xch(b);
					b.xch(r);
				}
				return a.is_one();
			}
			else
			{
				return false;
			}
			
		}
		
		
		
		natural  natural:: mod_inv( const natural &b, const natural &n )
		{
			//static const uint64_t _zero = 0, _one = 1;
			natural n0 = n;
			natural b0 = b;
			natural t0;// = _zero;
			natural t( one() );
			natural q  = n0/b0;
			natural r  = n0 - q*b0;
			
			while( !r.is_zero() )
			{
				const natural lhs = t0;
				const natural rhs = q*t;
				
				t0 = t;
				
				if( compare(lhs,rhs) >= 0 )
				{
					assert(lhs>=rhs);
					t = ( (lhs-rhs) % n );
				}
				else
				{
					assert(rhs>lhs);
					t  = n - ( (rhs-lhs)%n );
				}
				
				n0 = b0;
				b0 = r;
				q  = n0/b0;
				r  = n0 - q * b0;
			}
            // normally an error if b0 != 1
			return t;
		}
		
		natural natural:: mod_exp( const natural &B, const natural &E, const natural &N )
		{
			
			if( N.is_zero() )
				throw libc::exception( EDOM, "natural.exp %% 0");
			
			
			natural result( one() );
			if( E.size > 0 )
			{
				assert( E.bits() > 0 );
				natural        base  = B;
				const size_t   nbit  = E.bits()-1;
				const uint8_t *ebit  = E.byte;
				
				for( size_t i=0; i < nbit; ++i )
				{
                    
					if( ebit[i>>3] & _bit[ i & 7 ] )
					{
						//result = ( result * base ) % N;
						natural tmp1( __mod( mul( result, base ), N ) );
						tmp1.xch( result );
					}
					//base = ( base * base ) % N;
                    natural tmp2( __mod( sqr( base ), N ) );
					base.xch( tmp2 );
				}
				
				//-- most significant bit !
				assert( (ebit[nbit>>3] & _bit[ nbit & 7 ]) != 0 );
				
				//result = ( result * base ) % N;
				natural tmp3( __mod( mul( result, base ), N ) );
				tmp3.xch( result );
			}
			
			return result;
			
		}
        
    }
    
}
