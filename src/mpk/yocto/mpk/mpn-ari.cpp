#include "yocto/mpk/natural.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>

namespace yocto
{
	namespace mpk
	{
		
		natural natural::gcd( const natural &x, const natural &y )
		{
			if( x.size_ > 0 && y.size_ > 0 )
			{
				
				natural a( x );
				natural b( y );
				while( b.size_ > 0 )
				{
					natural r( mod__( a, b ) );
					a.xch(b);
					b.xch(r);
				}
				return a;
			}
			else
			{
				static const uint8_t _one = 1;
				return natural( _one );
			}
		}
		
		bool natural::are_coprime( const natural &x, const natural &y )
		{
			if( x.size_ > 0 && y.size_ > 0 )
			{
				
				natural a( x );
				natural b( y );
				while( b.size_ > 0 )
				{
					natural r( mod__( a, b ) );
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
			static const uint64_t _zero = 0, _one = 1;
			natural n0 = n;
			natural b0 = b;
			natural t0 = _zero;
			natural t  = _one;
			natural q  = n0/b0;
			natural r  = n0 - q*b0;
			
			while( r.is_positive() )
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
			
			
			natural result(1);
			if( E.size_ > 0 )
			{
				assert( E.bits() > 0 );
				natural        base  = B;
				const size_t   nbit  = E.bits()-1; 
				const uint8_t *ebit  = E.byte_;
				
				for( size_t i=0; i < nbit; ++i )
				{

					if( ebit[i>>3] & _bit[ i & 7 ] )
					{
						//result = ( result * base ) % N;
						natural tmp1( mod__( fft_( result, base ), N ) );
						tmp1.xch( result );
					}
					//base = ( base * base ) % N;
                    natural tmp2( mod__( sqr_( base ), N ) );
					base.xch( tmp2 );
				}
				
				//-- most significant bit !
				assert( (ebit[nbit>>3] & _bit[ nbit & 7 ]) != 0 );
				
				//result = ( result * base ) % N;
				natural tmp3( mod__( fft_( result, base ), N ) );
				tmp3.xch( result );
			}
			
			return result;
			
		}
		
		
		
		
	}

}
