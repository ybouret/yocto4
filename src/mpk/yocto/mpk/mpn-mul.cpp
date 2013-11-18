#include "yocto/mpk/natural.hpp"
#include "yocto/code/unroll.hpp"
#include "yocto/code/bswap.hpp"

namespace yocto
{
	namespace mpk
	{
		natural operator*( const natural &lhs, const natural &rhs )
		{
			return natural::fft_( lhs, rhs );
		}
		
		natural & natural:: operator*=( const natural &rhs )
		{
			natural tmp = fft_( *this, rhs );
			xch( tmp );
			return *this;
		}
		
		
		natural natural:: mul_( const natural &lhs, const natural &rhs )
		{
			
			const size_t      nL  = lhs.size_;
			const uint8_t    *pL  = lhs.byte_;
			
			const std::size_t nR  = rhs.size_;
			const uint8_t    *pR  = rhs.byte_;
			
			if( nL>0 && nR>0 )
			{
				if( nR < nL )
				{
					cswap<size_t>( *(size_t *)&nL, *(size_t *)&nR );
					cswap( pL, pR );
				}
				
				const size_t      len = nL + nR;
				natural           res( len, as_capacity );
				uint8_t          *reg = res.byte_;
				//==================================================================
				// ENTER: outer loop
				//==================================================================
				for( register size_t i=nL; i >0; --i ) 
				{
					const size_t L = *(pL++);
					uint8_t     *P = reg++;
					//==============================================================
					// ENTER: inner loop
					//==============================================================
					unsigned       W = 0;
					const uint8_t *Q = pR;
					YOCTO_LOOP_(nR,
								uint8_t &Pj = *(P++);
								W   += ( L * (*(Q++)) ) + Pj;
								Pj   = uint8_t( W );
								W  >>= 8
								);
					P[0] = uint8_t(W);
					
					//==============================================================
					// LEAVE: inner loop
					//==============================================================
				}
				//==================================================================
				// LEAVE: outer loop
				//==================================================================
				
				res.update();
				return res;
			}
			else
				return natural();
			
		}
		
	}
	
}

