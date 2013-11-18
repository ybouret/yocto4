#include "yocto/mpk/natural.hpp"
#include "yocto/code/round.hpp"

//#include <iostream>

namespace yocto
{
	namespace mpk
	{
	
#if defined(__ICC)
#pragma warning ( disable : 2259 )
#endif	
		//======================================================================
		// SHL
		//======================================================================
		natural natural::shl_( const natural &lhs,  size_t n )
		{
			if( n <= 0 )
				return lhs;
			else 
			{
				const size_t l_bits = lhs.bits();
				const size_t t_bits = l_bits + n;
				const size_t t_size = YOCTO_ROUND8(t_bits) >> 3;
				
				natural ans( t_size, as_capacity );
				const  uint8_t *src = lhs.byte_;
				uint8_t        *dst = ans.byte_;
				size_t obit = t_bits;
				size_t ibit = l_bits;
				for( size_t i=l_bits; i>0; --i )
				{
					--ibit;
					--obit;
					if( src[ ibit >> 3 ] & _bit[ ibit & 7 ] )
					{
						dst[ obit >> 3 ] |= _bit[ obit & 7];
					}
					
				}
				ans.update();
				return ans;
			}
		}
		
		natural  natural:: operator<<( size_t n ) const
		{
			return natural::shl_( *this, n );
		}
		
		natural & natural::shl()
		{
			natural tmp = shl_( *this, 1 );
			xch( tmp );
			return *this;
		}
		
		//======================================================================
		// SHR
		//======================================================================
		
		//-- optimized >> 1
		natural &natural:: shr() throw()
		{
			
			if( size_ > 0 )
			{
				assert( size_> 0 );
				uint8_t     *b = byte_;
				const size_t n = size_ - 1;
				for( size_t i=0; i < n; )
				{
					uint8_t &B = b[i];
					B >>= 1;
					B |= ( ( b[++i] & 0x1 ) << 7 ); //!< avoids a test !
				}
				b[n] >>= 1;
				update();
			}
			
			return *this;
		}
		
		natural natural::shr_( const natural &lhs,  size_t n )
		{
			if( n <= 0 )
				return lhs;
			else
			{
				const size_t l_bits = lhs.bits();
				if( n >= l_bits )
				{
					return natural();
				}
				else
				{
					assert( n > 0      );
					assert( n < l_bits );
					const size_t t_bits = l_bits - n;
					const size_t t_size = YOCTO_ROUND8(t_bits) >> 3;
					
					natural ans( t_size, as_capacity );
					const  uint8_t *src = lhs.byte_;
					uint8_t        *dst = ans.byte_;
					size_t obit = t_bits;
					size_t ibit = l_bits;
					for( size_t i=t_bits; i>0; --i )
					{
						--ibit;
						--obit;
						if( src[ ibit >> 3 ] & _bit[ ibit & 7 ] )
						{
							dst[ obit >> 3 ] |= _bit[ obit & 7];
						}
						
					}
					assert( ans.byte_[ ans.size_ - 1] != 0 );
					return ans;
				}
			}
		}
		
		natural  natural:: operator>>( size_t n ) const
		{
			return natural::shr_( *this, n );
		}
		
		//======================================================================
		// 1 << n
		//======================================================================
		natural natural::exp2( size_t n)
		{
			const size_t nbits = n+1;
			const size_t bytes = YOCTO_ROUND8(nbits) >> 3;
			natural ans( bytes, as_capacity );
			ans.byte_[ bytes-1 ] = _bit[ n & 7 ];
			return ans;
		}
		
		
	}
	
}

