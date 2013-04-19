#include "yocto/mpk/natural.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/code/cast.hpp"

#include <cerrno>


namespace yocto
{
	namespace mpk
	{
		natural operator-( const natural &lhs, const natural &rhs )
		{
			return natural::sub_( lhs, rhs );
		}
		
		natural & natural:: operator-=( const natural &rhs )
		{
			natural tmp = sub_( *this, rhs );
			xch( tmp );
			return *this;
		}
		
		natural & natural:: operator--()
		{
			natural tmp = dec_(1);
			xch( tmp );
			return *this;
		}
		
		
		natural natural:: sub_( const natural &lhs, const natural &rhs )
		{
			
			const int cmp = natural::compare(lhs,rhs);
			if( cmp < 0 )
			{
				throw libc::exception( EDOM, "mpk.natural.sub(lhs<rhs)");
			}
			else {
				if( cmp == 0 )
				{
					return natural();
				}
				else
				{
					assert( lhs.size_ >= rhs.size_ );
					
					const size_t   nL = lhs.size_;
					natural        ans( nL, as_capacity );
					const size_t   nR = rhs.size_;
					const uint8_t *L  = lhs.byte_;
					const uint8_t *R  = rhs.byte_;
					uint8_t       *S  = ans.byte_;
					
					assert( nR <= nL );
					
					ptrdiff_t   carry = 0;
					for( size_t i=nR; i >0; --i )
					{
						const ptrdiff_t a = *(L++); // L[i];
						const ptrdiff_t b = *(R++); // R[i];
						ptrdiff_t       d = (a-b)-carry;
						if( d < 0 ) 
						{
							d    += 256;
							carry = 1;
						}
						else 
						{
							carry = 0;
						}
						assert(d>=0);
						assert(d<256);
						*(S++) = static_cast<uint8_t>(d);
					}
					
					for( size_t i=nL-nR; i>0; --i )
					{
						const ptrdiff_t a = *(L++);  //L[i];
						ptrdiff_t       d = a-carry;
						if( d < 0 ) 
						{
							d    += 256;
							carry = 1;
						}
						else 
						{
							carry = 0;
						}
						assert(d>=0);
						assert(d<256);
						*(S++) = static_cast<uint8_t>(d);
					}
					
					
					ans.update();
					return ans;
				}
			}
			
		}
	
		natural natural:: dec_( uint8_t x ) const
		{
			uint64_t wksp[ YOCTO_U64_FOR_ITEM(natural) ];
			natural &rhs = *_cast::trans<natural,uint64_t>(wksp);
			rhs.size_ = 1;
			rhs.byte_ = &x;
			return sub_( *this, rhs );
		}
		
		natural & natural:: dec( uint8_t x)
		{
			natural tmp( dec_(x) );
			xch( tmp );
			return *this;
		}
		
	}
	
}
