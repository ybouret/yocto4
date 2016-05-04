#include "yocto/math/v2d.hpp"
#include "yocto/math/types.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/code/utils.hpp"
#include <iostream>
#include <cstdlib>

namespace yocto {
	
	namespace math {
        
        
		template <>
		v2d<real_t>:: v2d() throw() : x(0), y(0) {
		}
		
		template <>
		v2d<real_t>:: v2d( const real_t X, const real_t Y) throw() : x(X), y(Y) {
		}
		
		template <>
		v2d<real_t>:: v2d( const v2d<real_t> &v ) throw() : x(v.x), y(v.y) {
		}
		
        template<> const v2d<real_t> v2d<real_t>::zero(0,0);

		template <>
		v2d<real_t>:: v2d( const v2d<real_t> &A, const v2d<real_t> &B ) throw() : x(B.x - A.x), y(B.y-A.y) {
		}
		
		
		template <>
		v2d<real_t> &v2d<real_t>::operator=( const v2d<real_t> &v ) throw() {
			x = v.x;
			y = v.y;
			return *this;
		}
		
		// +
		template <>
		v2d<real_t> &v2d<real_t>:: operator+=( const v2d<real_t> &v) throw() {
			x += v.x;
			y += v.y;
			return *this;
		}
		
		template <>
		v2d<real_t> v2d<real_t>:: operator+() const throw() {
			return v2d<real_t>(x,y);
		}
		
		template <>
		v2d<real_t> v2d<real_t>:: operator+( const v2d<real_t> &v ) const throw() {
			return v2d<real_t>(x+v.x,y+v.y);
		}
		
		//-
		template <>
		v2d<real_t> &v2d<real_t>:: operator-=( const v2d<real_t> &v) throw() {
			x -= v.x;
			y -= v.y;
			return *this;
		}
		
		template <>
		v2d<real_t> v2d<real_t>:: operator-() const throw() {
			return v2d<real_t>(-x,-y);
		}
		
		template <>
		v2d<real_t> v2d<real_t>:: operator-( const v2d<real_t> &v ) const throw() {
			return v2d<real_t>(x-v.x,y-v.y);
		}
		
		// *
		template <>
		v2d<real_t> &v2d<real_t>:: operator*=( const real_t a) throw() {
			x *= a;
			y *= a;
			return *this;
		}
		
		
		
		template <>
		v2d<real_t> v2d<real_t>::mul_( const real_t a, const v2d<real_t> &v) throw() {
			return v2d<real_t>(a * v.x, a * v.y);
		}
		
		template <>
		real_t v2d<real_t>::dot_( const v2d<real_t> &A, const v2d<real_t> &B ) throw() {
			return (A.x * B.x) + (A.y * B.y);
		}
		
		template <>
		real_t v2d<real_t>:: norm2() const throw() {
			return x*x + y*y;
		}
		
		
		
		template <>
		v2d<real_t> &v2d<real_t>:: operator/=( const real_t a) {
			x /= a;
			y /= a;
			return *this;
		}
		
		template <>
		v2d<real_t> v2d<real_t>::div_(  const v2d<real_t> &v, const real_t a)  {
			return v2d<real_t>(v.x/a,v.y/a);
		}
		
		
#if YOCTO_ZTYPE != 'i'
		template <>
		real_t v2d<real_t>:: norm() const throw() {
			return   math::Hypotenuse(x, y);
		}
        
        template <>
		real_t v2d<real_t>:: angle() const throw() {
			return math::Atan2(x,y);
		}
		
        template <>
        real_t  v2d<real_t>::positive_angle() const throw()
        {
            const real_t a = math::Atan2(x,y);
            return a < 0 ? a + math::numeric<real_t>::two_pi : a;
        }
        
        template <>
        real_t v2d<real_t>:: dist2(const v2d<real_t> &lhs, const v2d<real_t> &rhs ) throw()
        {
            const real_t dx = rhs.x - lhs.x;
            const real_t dy = rhs.y - lhs.y;
            return dx*dx + dy*dy;
        }
        
        template <>
        real_t v2d<real_t>:: dist(const v2d<real_t> &lhs, const v2d<real_t> &rhs ) throw()
        {
            const real_t dx = rhs.x - lhs.x;
            const real_t dy = rhs.y - lhs.y;
            return Hypotenuse(dx, dy);
        }
        
#else
		template <>
		real_t v2d<real_t>:: norm() const throw() {
            return  std::abs(x) +  std::abs(y);
		}
#endif
		
#if YOCTO_ZTYPE != 'i'
        
		template <>
		void v2d<real_t>:: normalize() throw() {
			const real_t fac =  REAL(1.0) / math::Hypotenuse(x,y);
			x *= fac;
			y *= fac;
		}
		
		
		template <>
		v2d<real_t>::v2d( Random::Uniform &ran ) throw() :
		x( 0 ),
		y( 0 )
		{
			const real_t theta = REAL_TWO_PI * real_t(ran());
			x =  math::Cos( theta );
			y =  math::Sin( theta );
		}
		
		template <>
		v2d<real_t>::v2d( Random::Uniform &ran, const real_t radius ) throw() :
		x( 0 ),
		y( 0 )
		{
			const real_t theta =  REAL_TWO_PI * real_t(ran());
			x =  radius * math::Cos( theta );
			y =  radius * math::Sin( theta );
		}
		
        template <>
        real_t v2d<real_t>::angle_of( const v2d<real_t> &a, const v2d<real_t> &b ) throw()
        {
            const real_t C = clamp<real_t>(-1,(a*b)/(a.norm()*b.norm()),1);
            const real_t S = a.x * b.y - a.y * b.x;
            return math::Signed<real_t,real_t>( math::Acos(C), S );
        }
        
        template <>
        v2d<real_t> v2d<real_t>:: rotated_by( real_t angle ) const throw()
        {
            const real_t c = Cos(angle);
            const real_t s = Sin(angle);
            return v2d<real_t>( c * x - s * y, s*x + c *y);
        }

        
#endif
		
        template <>
        real_t v2d<real_t>::det( const v2d<real_t> &a, const v2d<real_t> &b ) throw()
        {
            return a.x * b.y - a.y * b.x;
        }
        
        template <>
        void v2d<real_t>:: ldz() throw()
        {
            x=y=0;
        }
        
		template <>
		void v2d<real_t>:: output( std::ostream &os ) const {
			os << "[ " << x << ' ' << y << " ]'";
		}
		
		template <>
		real_t & v2d<real_t>:: operator[]( size_t i ) throw()
		{
			assert(i<2);
			return *( &x + i);
		}
		
		template <>
		const real_t & v2d<real_t>:: operator[]( size_t i ) const throw()
		{
			assert(i<2);
			return *( &x + i);
		}
		
	} //math
	
} // yocto
