#include "yocto/mpa/natural.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>

namespace yocto
{
    namespace mpa
    {
        natural operator/( const natural &lhs, const natural &rhs )
		{
			return natural::div( lhs, rhs );
		}
		
		natural & natural:: operator/=( const natural &rhs )
		{
			natural tmp = div( *this, rhs );
			xch( tmp );
			return *this;
		}
		
		namespace
        {
            enum natural_comparison
            {
                natural_is_less,
                natural_is_equal,
                natural_is_greater
            };
            
            static inline natural_comparison int2cmp( int cmp ) throw()
            {
                if(cmp<0)
                    return natural_is_less;
                else
                {
                    if(cmp>0)
                        return natural_is_greater;
                    else
                        return natural_is_equal;
                }
            }
        }
        
		natural  natural::div( const natural &num, const natural &den )
		{
			
			//------------------------------------------------------------------
			//-- check error
			//------------------------------------------------------------------
			if( den.is_zero() )
				throw libc::exception( EDOM, "mpa(DIV by Zero)");
			
			//------------------------------------------------------------------
			//-- get rid of trivial cases
			//------------------------------------------------------------------
			switch( int2cmp(natural::compare( num, den )) )
			{
				case natural_is_less:    return natural();
				case natural_is_equal:   return natural::one();
				case natural_is_greater: break;
			}
			
			//------------------------------------------------------------------
			// Look Up
			//------------------------------------------------------------------
			assert( num > den );
			size_t p = 1;
			{
				natural probe = add(den,den); //! 2^(p=1) * den
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
				natural mid = add( lo, hi ).shr();
				const natural tmp = mul( mid, den );
				switch( int2cmp(natural::compare(tmp,num)) )
				{
					case natural_is_less:
						lo.xch(mid);
						break;
						
					case natural_is_equal:
						return mid; // very special case
						
					case natural_is_greater:
						hi.xch(mid);
						break;
				}
			}
			return lo;
		}
        
        
    }
    
}

