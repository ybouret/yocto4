
#include "yocto/math/m2d.hpp"
#include "yocto/math/types.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/code/bswap.hpp"

#include <iostream>

namespace yocto {
    
	namespace math {
        
		
		template<>
		m2d<real_t>:: m2d(const real_t diag) throw() :
        ex(diag,0),
        ey(0,diag)
		{
		}
        
		template <>
		m2d<real_t>:: ~m2d() throw() {
		}
        
        
		template<>
		m2d<real_t>:: m2d( const v2d<real_t> &vx, const v2d<real_t> &vy ) throw() :
        ex(vx),
        ey(vy)
		{
            
		}
        
		template<>
		m2d<real_t>:: m2d( const v2d<real_t> &diag) throw() :
        ex( diag.x, 0 ),
        ey( 0, diag.y )
		{
		}
        
		template<>
		m2d<real_t>:: m2d( const m2d<real_t> &m ) throw() :
        ex( m.ex ),
        ey( m.ey )
		{
		}
        
        
        
		template<>
		m2d<real_t> & m2d<real_t>::operator=( const m2d<real_t> &m ) throw()  {
			ex = m.ex;
			ey = m.ey;
			return *this;
		}
        
        
		//----------------------------------------------------------------------
		// +
		//----------------------------------------------------------------------
		template <>
		m2d<real_t> & m2d<real_t>:: operator+=( const m2d<real_t> &m ) throw() {
			ex += m.ex;
			ey += m.ey;
			return *this;
		}
        
		template <>
		m2d<real_t> m2d<real_t>:: operator+() const throw() {
			return m2d<real_t>( *this );
		}
        
		template <>
		m2d<real_t> m2d<real_t>:: operator+(const m2d<real_t> &m ) const throw() {
			return m2d<real_t>( ex + m.ex, ey + m.ey );
		}
        
		//----------------------------------------------------------------------
		// -
		//----------------------------------------------------------------------
		template <>
		m2d<real_t> & m2d<real_t>:: operator-=( const m2d<real_t> &m ) throw() {
			ex -= m.ex;
			ey -= m.ey;
			return *this;
		}
        
		template <>
		m2d<real_t> m2d<real_t>:: operator-() const throw() {
			return m2d<real_t>( -ex, -ey );
		}
        
		template <>
		m2d<real_t> m2d<real_t>:: operator-(const m2d<real_t> &m ) const throw() {
			return m2d<real_t>( ex - m.ex, ey - m.ey );
		}
        
		template <>
		m2d<real_t> & m2d<real_t>:: operator *= ( const real_t a ) throw() {
			ex *= a;
			ey *= a;
			return *this;
		}
        
        
        
		template <>
		m2d<real_t>  operator*(const real_t a, const m2d<real_t> &m) throw() {
			m2d<real_t> ans( m );
			return (ans *= a);
		}
        
		template <>
		m2d<real_t>  operator*(const m2d<real_t> &a, const m2d<real_t> &b) throw() {
			const v2d<real_t> vx( a.ex.x * b.ex.x + a.ey.x * b.ex.y,  a.ex.y * b.ex.x + a.ey.y * b.ex.y );
			const v2d<real_t> vy( a.ex.x * b.ey.x + a.ey.x * b.ey.y,  a.ex.y * b.ey.x + a.ey.y * b.ey.y );
			return m2d<real_t>(vx,vy);
		}
        
		template <>
		m2d<real_t> & m2d<real_t>:: operator *= ( const m2d<real_t> &m ) throw() {
			return ( *this = ( (*this) *  m ) );
		}
        
        
		template <>
		real_t m2d<real_t>:: det(void) const throw() {
			return ex.x * ey.y - ex.y * ey.x;
		}
        
		template <>
		void  m2d<real_t>:: ortho() throw() {
			ex.normalize();
			ey.x = -ex.y;
			ey.y =  ex.x;
		}
        
		template <>
		void  m2d<real_t>:: transpose() throw() {
			cswap( ex.y, ey.x );
		}
        
		template <>
		void m2d<real_t>:: inverse() throw() {
			const real_t d = this->det();
            
            
			const real_t fac = real_t(1)/d;
			const real_t xx  =  ey.y * fac;
			const real_t yy  =  ex.x * fac;
			const real_t xy  = -ex.y * fac;
			const real_t yx  = -ey.x * fac;
            
			ex.x = xx;
			ex.y = xy;
            
			ey.x = yx;
			ey.y = yy;
            
            
		}
        
		template <>
		std::ostream &m2d<real_t>::to_scilab( std::ostream &os ) const
		{
			return os << "[ " << ex.x << " " << ey.x << " ; "
            /* */      << ex.y << " " << ey.y << " ]";
		}
        
		template <>
		void m2d<real_t>:: symmetric() throw() {
			const real_t x = REAL(0.5) * (ex.y + ey.x);
			ex.y = ey.x = x;
		}
        
		template <>
		real_t m2d<real_t>:: jacobi( m2d<real_t> &q, v2d<real_t> &lam ) throw() {
			const real_t two_b = ex.y + ey.x;
			real_t b           = REAL(0.5) * two_b;
			const real_t a     = ex.x;
			const real_t c     = ey.y;
			const real_t diff  = a-c;
			const real_t abs_two_b = math::Fabs( two_b );
			const real_t abs_diff  = math::Fabs( diff  );
            
			real_t       t     = 0;
			if( abs_two_b > REAL_MIN && abs_two_b > REAL_EPSILON*(std::fabs(a)+std::fabs(c) ) ) {
                
				if( abs_diff < abs_two_b ) {
					const real_t rho = diff / two_b;
					t = REAL(1.0)/( std::fabs(rho) + std::sqrt( REAL(1.0) + rho * rho ) );
					if( rho < 0 )
						t = -t;
				}
				else {
					// abs_diff >= abs_two_b > REAL_MIN
					const real_t sigma = two_b / diff;
					t = sigma/(REAL(1.0) + math::Sqrt( REAL(1.0) + sigma * sigma ));
				}
                
			}
			// else do nothing
            
            
			const real_t theta = std::atan(t);
			const real_t C_     = std::cos( theta );
			const real_t S_     = std::sin( theta );
			const real_t C2_    = C_*C_;
			const real_t S2_    = S_*S_;
			const real_t CS_    = C_*S_;
            
			//-- update q
			q.ex.x =  C_;
			q.ex.y =  S_;
			q.ey.x = -S_;
			q.ey.y =  C_;
            
			//-- this is symmetric
			ex.y = ey.x = b;
            
			//-- compute eigenvalues
			const real_t dum = two_b * CS_;
			lam.x = a*C2_ + c*S2_ + dum;
			lam.y = a*S2_ + c*C2_ - dum;
            
            
			return theta;
            
            
		}
		
		
		template <>
		void m2d<real_t>::mul( v2d<real_t> &target, const v2d<real_t> &source) const throw()
		{
			assert( &target != &source );
			target.x = ex.x * source.x + ey.x * source.y;
			target.y = ex.y * source.x + ey.y * source.y;
            
		}
		
		template <>
		void m2d<real_t>::mul( v2d<real_t> &source ) const throw()
		{
			const real_t target_x = ex.x * source.x + ey.x * source.y;
			const real_t target_y = ex.y * source.x + ey.y * source.y;
			source.x = target_x;
			source.y = target_y;
		}
		
		
        
	}
    
}

