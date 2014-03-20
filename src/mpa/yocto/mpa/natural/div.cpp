#include "yocto/mpa/word2mpn.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>

#include <iostream>

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
        
        natural  natural:: div( const natural &num, const uint64_t x   )
        {
            const word2mpn w(x);
            return div(num,w.n);
        }

        natural operator/( const natural &num, const uint64_t den )
        {
            return natural::div(num,den);
        }
        
        natural  natural:: div( const uint64_t x,   const natural &den )
        {
            const word2mpn w(x);
            return div(w.n,den);
        }
        
        natural operator/( const uint64_t num, const natural &den )
        {
            return natural:: div(num,den);
        }
        
        natural & natural:: operator/=( const uint64_t den )
        {
            natural tmp = div( *this, den );
			xch( tmp );
			return *this;
        }
        
    }
    
}

namespace yocto
{
    
    namespace mpa
    {
        
        natural operator%( const natural &lhs, const natural &rhs )
		{
			return natural::modulo( lhs, rhs );
		}
		
		natural & natural:: operator%=( const natural &rhs )
		{
			natural tmp = modulo( *this, rhs );
			xch( tmp );
			return *this;
		}
		
		natural  natural::modulo( const natural &num, const natural &den )
		{
			if( den.is_zero() )
				throw libc::exception( EDOM, "modulo(DIV by Zero)");
			return __mod( num, den );
		}
		
		natural  natural::__mod( const natural &num, const natural &den )
		{
			
			//------------------------------------------------------------------
			//-- check error
			//------------------------------------------------------------------
			assert( !den.is_zero() );
			
			
			//------------------------------------------------------------------
			//-- get rid of trivial cases
			//------------------------------------------------------------------
			switch( int2cmp( compare(num, den) ) )
			{
				case natural_is_less:    return num;       //
				case natural_is_equal:   return natural(); // 0, easy...
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
				switch( int2cmp(compare(tmp,num)) )
				{
					case natural_is_less:
						lo.xch(mid);
						break;
						
					case natural_is_equal:
						return natural(); // very special case
						
					case natural_is_greater:
						hi.xch(mid);
						break;
				}
			}
			const natural prod = mul(lo,den); assert( prod <= num );
			return sub( num, prod );
		}
        
        bool natural:: is_divisible_by( const natural &den ) const
        {
            if( den.is_zero() )
                throw libc::exception( EDOM, "DIV by Zero");
            
            const natural &num = *this;
            
            //------------------------------------------------------------------
			//-- get rid of trivial cases
			//------------------------------------------------------------------
			switch( int2cmp( compare(num, den) ) )
			{
				case natural_is_less:
                    return num.is_zero();  // easy...

				case natural_is_equal:
                    return true;    // easy...
				
                case natural_is_greater:
                    break;          // need to test
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
				switch( int2cmp(compare(tmp,num)) )
				{
					case natural_is_less:
						lo.xch(mid);
						break;
						
					case natural_is_equal:
						return true; // very special case
						
					case natural_is_greater:
						hi.xch(mid);
						break;
				}
			}
            const natural prod = mul(lo,den);
            return 0 == compare(num,prod);
        }
        
        
    }
    
}

