#include "yocto/mpk/natural.hpp"

#include "yocto/mpk/natural.hpp"

#include "yocto/exceptions.hpp"
#include <cerrno>


namespace yocto
{
	namespace mpk
	{
		
		natural operator%( const natural &lhs, const natural &rhs )
		{
			return natural::mod_( lhs, rhs );
		}
		
		natural & natural:: operator%=( const natural &rhs )
		{
			natural tmp = mod_( *this, rhs );
			xch( tmp );
			return *this;
		}
		
		natural  natural::mod_( const natural &num, const natural &den )
		{
			if( den.is_zero() ) 
				throw libc::exception( EDOM, "mod(DIV by Zero)");
			return mod__( num, den );
		}
		
		natural  natural::mod__( const natural &num, const natural &den )
		{
			
			//------------------------------------------------------------------
			//-- check error
			//------------------------------------------------------------------
			assert( den.is_positive() );
			
			
			//------------------------------------------------------------------
			//-- get rid of trivial cases
			//------------------------------------------------------------------
			switch( natural::comparison_of( num, den ) )
			{
				case natural::is_less:    return num;       //
				case natural::is_equal:   return natural(); // 0, easy...
				case natural::is_greater: break;
			}
			
			//------------------------------------------------------------------
			// Look Up
			//------------------------------------------------------------------
			assert( num > den );
			size_t p = 1;
			{
				natural probe = add_(den,den); //! 2^(p=1) * den
				while( compare(probe,num) <= 0 )
				{
					++p;
					probe.shl();
				}
				assert( probe > num );
			}
			
			natural hi = natural::exp2(   p ); //assert( num < mul_(hi,den) );
			natural lo = natural::exp2( --p ); //assert( mul_(lo,den) <= num );
			
			
			while( p-- > 0 )	
			{
				//--------------------------------------------------------------
				// Bisection
				//--------------------------------------------------------------
				natural mid = add_( lo, hi ).shr();
				const natural tmp = fft_( mid, den );
				switch( comparison_of(tmp,num) )
				{
					case is_less:
						lo.xch(mid);
						break;
						
					case is_equal:
						return natural(); // very special case
						
					case is_greater:
						hi.xch(mid);
						break;
				}
			}
			const natural prod = fft_(lo,den); assert( prod <= num );
			return sub_( num, prod );
		}
		
		
				
	}
	
}
