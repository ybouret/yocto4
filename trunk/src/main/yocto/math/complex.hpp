#ifndef YOCTO_MATH_COMPLEX_INCLUDED
#define YOCTO_MATH_COMPLEX_INCLUDED 1

#include "yocto/bitwise.hpp"

#include <cfloat>
#include <iostream>

namespace yocto 
{
	
	namespace math
	{
		template <typename T>
		struct complex
		{
			T re, im;
			
			
			
			complex() throw();
			complex( const T x ) throw();
			complex( const T x, const T y ) throw();
			complex( const complex &z ) throw();
			
			complex & operator=( const complex &z ) throw();
			complex & operator=( const T        x ) throw();
			
			~complex() throw();
			
			//------------------------------------------------------------------
			// addition
			//------------------------------------------------------------------
			complex & operator+=( const complex & ) throw();
			complex & operator+=( const T         ) throw();
			
			complex   operator+() const throw();
			complex   operator+( const complex &rhs ) const throw();
			complex   operator+( const T rhs ) const throw();
			friend inline 
			complex operator+( const T lhs, const complex &rhs ) throw()
			{
				return complex( lhs + rhs.re, rhs.im );
			}
			
			
			//------------------------------------------------------------------
			// subtraction
			//------------------------------------------------------------------
			complex & operator-=( const complex & ) throw();
			complex & operator-=( const T         ) throw();
			
			complex   operator-() const throw();
			complex   operator-( const complex &rhs ) const throw();
			complex   operator-( const T rhs ) const throw();
			friend inline 
			complex operator-( const T lhs, const complex &rhs ) throw()
			{
				return complex( lhs - rhs.re, rhs.im );
			}
			
			//------------------------------------------------------------------
			// multiplication
			//------------------------------------------------------------------
			complex & operator*=( const complex & ) throw();
			complex & operator*=( const T         ) throw();
			
			complex   operator*( const complex &rhs ) const throw();
			complex   operator*( const T rhs ) const throw();
			friend inline 
			complex operator*( const T lhs, const complex &rhs ) throw()
			{
				return complex( lhs * rhs.re, lhs * rhs.im );
			}
			
			//------------------------------------------------------------------
			// conjugaison
			//------------------------------------------------------------------
			complex conj() const throw();
			
			//------------------------------------------------------------------
			// modulus
			//------------------------------------------------------------------
			T mod2() const throw();
			T mod()  const throw();
			T arg()  const throw();
            
			//------------------------------------------------------------------
			// division
			//------------------------------------------------------------------
			complex & operator/=( const complex & ) throw();
			complex & operator/=( const T         ) throw();
			
			complex   operator/( const complex &rhs ) const throw();
			complex   operator/( const T rhs ) const throw();
			friend inline 
			complex operator/( const T lhs, const complex &rhs ) throw()
			{
				return complex(lhs)/rhs;
			}
			
			friend std::ostream & operator<<( std::ostream &_os, const complex &C )
			{
				_os << '(' << C.re << ',' << C.im << ')';
				return _os;
			}
			
			friend 
			inline  bool operator != ( const complex &lhs, const complex &rhs ) throw()
			{
				return (lhs.re != rhs.re) || (lhs.im != rhs.im);
			}
			
            inline void in_place_squared() throw()
            {
                const T a = re * re - im * im;
                const T b = re * im;
                re = a;
                im = b+b;
            }
		};
		
		
		
	}
	
}


YOCTO_SUPPORT_C_STYLE_OPS( math::complex<float>  )
YOCTO_SUPPORT_C_STYLE_OPS( math::complex<double> )

#endif
