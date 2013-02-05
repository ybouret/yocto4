#include "yocto/math/ztype.hpp"
#include "yocto/math/types.hpp"

namespace yocto 
{
	
	namespace math
	{
		
		template <> complex<real_t>:: complex() throw() : re(0), im(0) {}
		template <> complex<real_t>:: complex( const real_t x ) throw() : re( x ), im( 0 ) {}
		template <> complex<real_t>:: complex( const real_t x, const real_t y ) throw() : re( x ), im( y ) {}
		template <> complex<real_t>:: complex( const complex &z ) throw() : re( z.re ), im( z.im ) {}
		template <> complex<real_t> & complex<real_t>:: operator=( const complex<real_t> & z ) throw() { re = z.re; im = z.im; return *this; }
		template <> complex<real_t> & complex<real_t>:: operator=( const real_t            x ) throw() { re = x;    im = 0;    return *this; }
		template <> complex<real_t>:: ~complex() throw() {}
		
		
		//-- add
		template <> complex<real_t> & complex<real_t>:: operator+=( const complex<real_t> &rhs ) throw()
		{
			re += rhs.re;
			im += rhs.im;
			return *this;
		}
		
		template <> complex<real_t> & complex<real_t>:: operator+=( const real_t rhs ) throw()
		{
			re += rhs;
			return *this;
		}
		
		template <> complex<real_t> complex<real_t>:: operator+() const throw()
		{
			return *this;
		}
		
		template <> complex<real_t> complex<real_t>:: operator+( const complex<real_t> &rhs ) const throw()
		{
			return complex<real_t>( re + rhs.re, im+rhs.im );
		}
		
		template <> complex<real_t> complex<real_t>:: operator+( const real_t rhs ) const throw()
		{
			return complex<real_t>( re + rhs, im );
		}
		
		//-- sub
		template <> complex<real_t> & complex<real_t>:: operator-=( const complex<real_t> &rhs ) throw()
		{
			re -= rhs.re;
			im -= rhs.im;
			return *this;
		}
		
		template <> complex<real_t> & complex<real_t>:: operator-=( const real_t rhs ) throw()
		{
			re -= rhs;
			return *this;
		}
		
		template <> complex<real_t> complex<real_t>:: operator-() const throw()
		{
			return complex( -re, -im );
		}
		
		template <> complex<real_t> complex<real_t>:: operator-( const complex<real_t> &rhs ) const throw()
		{
			return complex<real_t>( re - rhs.re, im - rhs.im );
		}
		
		template <> complex<real_t> complex<real_t>:: operator-( const real_t rhs ) const throw()
		{
			return complex<real_t>( re - rhs, im );
		}
		
		//-- mul
		template <> complex<real_t> & complex<real_t>:: operator*=( const complex<real_t> &rhs ) throw()
		{
			const real_t a = re * rhs.re - im * rhs.im;
			const real_t b = re * rhs.im + im * rhs.re;
			re = a;
			im = b;
			return *this;
		}
		
		template <> complex<real_t> & complex<real_t>:: operator*=( const real_t rhs ) throw()
		{
			re *= rhs;
			im *= rhs;
			return *this;
		}
		
		
		
		template <> complex<real_t> complex<real_t>:: operator*( const complex<real_t> &rhs ) const throw()
		{
			return complex<real_t>( re * rhs.re - im * rhs.im, re * rhs.im + im * rhs.re );
		}
		
		template <> complex<real_t> complex<real_t>:: operator*( const real_t rhs ) const throw()
		{
			return complex<real_t>( re * rhs, im * rhs);
		}
		
		//-- modulus
		template<> real_t complex<real_t>:: mod2() const throw() { return re * re + im * im; }
		template<> real_t complex<real_t>:: mod() const throw()  { return Sqrt( re * re + im * im ); }
		
        template<> real_t complex<real_t>:: arg() const throw()  { return Atan2(re,im); }

        
		//-- conj
		template <>
		complex<real_t> complex<real_t>:: conj() const throw()
		{
			return complex( re, -im );
		}
		
		
		//-- div
		template <> complex<real_t> & complex<real_t>:: operator/=( const complex<real_t> &rhs ) throw()
		{
			complex      tmp = (*this) * ( rhs.conj());
			const real_t den = rhs.mod2();
		
			re = tmp.re / den;
			im = tmp.im / den;
			
			return *this;
		}
		
		template <> complex<real_t> & complex<real_t>:: operator/=( const real_t rhs ) throw()
		{
			re /= rhs;
			im /= rhs;
			return *this;
		}
		
		
		
		template <> complex<real_t> complex<real_t>:: operator/( const complex<real_t> &rhs ) const throw()
		{
			complex lhs = *this;
			return complex(lhs /= rhs);
		}
		
		template <> complex<real_t> complex<real_t>:: operator/( const real_t rhs ) const throw()
		{
			return complex<real_t>( re / rhs, im / rhs);
		}
		
		
	}
	
}
