#include "yocto/mpa/natural.hpp"
#include "yocto/code/bswap.hpp"
#include "yocto/code/round.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>

namespace yocto
{
    namespace mpa
    {
        natural natural:: sub( const natural &lhs, const natural &rhs )
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
					assert( lhs.size >= rhs.size );
					
					const size_t   nL = lhs.size;
					natural        ans( nL, as_capacity );
					const size_t   nR = rhs.size;
					const uint8_t *L  = lhs.byte;
					const uint8_t *R  = rhs.byte;
					uint8_t       *S  = ans.byte;
					
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
					
					
					ans.rescan();
					return ans;
				}
			}
            
        }
        
        
        natural operator-( const natural &lhs, const natural &rhs )
        {
            return natural::sub(lhs,rhs);
        }
        
        natural & natural:: operator-=( const natural &rhs )
        {
            natural tmp( sub(*this,rhs) );
            xch(tmp);
            return *this;
        }
        
        natural & natural:: operator--()
        {
            dec(1);
            return *this;
        }
        
        natural natural:: operator-- (int)
        {
            const natural saved( *this );
            dec(1);
            return saved;
        }
        
        void natural:: dec( uint8_t b )
        {
            if(b)
            {
                uint64_t wksp[ YOCTO_U64_FOR_ITEM(natural) ];
                natural *rhs = (natural *)&wksp[0];
                rhs->maxi    = 1;
                rhs->size    = 1;
                rhs->byte    = &b;
                natural ans( sub(*this,*rhs) );
                xch(ans);
            }
        }

        
    }
    
}


