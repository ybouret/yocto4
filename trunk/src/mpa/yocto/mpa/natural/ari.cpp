#include "yocto/mpa/natural.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>
#include "yocto/code/bswap.hpp"

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
            if(lhs.size>0 && rhs.size>0)
            {
                natural a(lhs);
                {
                    natural b(rhs);
                    while(b.size>0)
                    {
                        natural r( __mod(a,b) );
                        a.xch(b);
                        b.xch(r);
                    }
                }
                if(a>1)
                {
                    natural       L = natural::div(lhs,a);
                    natural       R = natural::div(rhs,a);
                    lhs.xch(L);
                    rhs.xch(R);
                }
            }
            
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
        
        natural natural:: binomial(uint64_t n, uint64_t p)
        {
            if(p>n) throw libc::exception( EDOM, "natural::binomial(p>n)");
            natural  N = one();
            uint64_t A = p;
            uint64_t B = n-p;
            if(A>B) cswap(A,B);
            assert(A<=B);
            natural D = factorial(A);
            for(uint64_t i=n;i>1;--i)
            {
                N *= i;
                simplify(N,D);
            }
            for(uint64_t i=B;i>1;--i)
            {
                D *= i;
                simplify(N, D);
            }
            assert(D==1);
            return N;
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
