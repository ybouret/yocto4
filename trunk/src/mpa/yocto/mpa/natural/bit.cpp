#include "yocto/mpa/natural.hpp"
#include "yocto/code/bswap.hpp"
#include "yocto/code/round.hpp"

namespace yocto
{
    namespace mpa
    {
       

        natural natural:: exp2(size_t n)
        {
            
            const size_t nbits = n+1;
			const size_t bytes = YOCTO_ROUND8(nbits) >> 3;
			natural ans( bytes, as_capacity );
			ans.byte[ bytes-1 ] = _bit[ n & 7 ];
            ans.size = bytes;
			return ans;
            
        }
        
        natural & natural:: shr() throw()
        {
            if( size > 0 )
            {
                uint8_t     *b = byte;
                const size_t n = size - 1;
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
        
        natural natural::shr( const natural &lhs,  size_t n )
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
					const  uint8_t *src = lhs.byte;
					uint8_t        *dst = ans.byte;
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
                    ans.size = t_size;
					assert( ans.byte[ ans.size - 1] != 0 );
					return ans;
				}
			}
		}
        
        natural natural:: operator>>( size_t n) const
        {
            return shr( *this, n );
        }

        natural & natural:: operator>>=( size_t n )
        {
            natural tmp( shr(*this,n) );
            xch(tmp);
            return *this;
        }

        
    }
    
}

