#include "yocto/math/v3d.hpp"
#include "yocto/math/types.hpp"
#include "yocto/math/ztype.hpp"
#include <iostream>
#include <cstdlib>

namespace yocto {
	
	namespace math {
		
		
		template <>
		v3d<real_t>:: v3d() throw() : x(0), y(0), z(0) {
		}
		
		template <>
		v3d<real_t>:: v3d( const real_t X, const real_t Y, const real_t Z) throw() : x(X), y(Y), z(Z) {
		}
		
		template <>
		v3d<real_t>:: v3d( const v3d<real_t> &v ) throw() : x(v.x), y(v.y), z(v.z) {
		}
		
		template <>
		v3d<real_t>:: v3d( const v3d<real_t> &A, const v3d<real_t> &B ) throw() : x(B.x - A.x), y(B.y-A.y), z(B.z-A.z) {
		}
		template <>
        void v3d<real_t>::ldz() throw()
        {
            x=y=z=0;
        }
        
		template <>
		v3d<real_t> &v3d<real_t>::operator=( const v3d<real_t> &v ) throw() {
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}
		
		// +
		template <>
		v3d<real_t> &v3d<real_t>:: operator+=( const v3d<real_t> &v) throw() {
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}
		
		template <>
		v3d<real_t> v3d<real_t>:: operator+() const throw() {
			return v3d<real_t>(x,y,z);
		}
		
		template <>
		v3d<real_t> v3d<real_t>:: operator+( const v3d<real_t> &v ) const throw() {
			return v3d<real_t>(x+v.x,y+v.y,z+v.z);
		}
		
		//-
		template <>
		v3d<real_t> &v3d<real_t>:: operator-=( const v3d<real_t> &v) throw() {
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}
		
		template <>
		v3d<real_t> v3d<real_t>:: operator-() const throw() {
			return v3d<real_t>(-x,-y,-z);
		}
		
		template <>
		v3d<real_t> v3d<real_t>:: operator-( const v3d<real_t> &v ) const throw() {
			return v3d<real_t>(x-v.x,y-v.y,z-v.z);
		}
		
		// *
		template <>
		v3d<real_t> &v3d<real_t>:: operator*=( const real_t a) throw() {
			x *= a;
			y *= a;
			z *= a;
			return *this;
		}
		
		template <>
		v3d<real_t> v3d<real_t>::mul_( const real_t a, const v3d<real_t> &v) throw() {
			return v3d<real_t>(a *v.x, a * v.y, a * v.z);
		}
		
        template <>
        v3d<real_t> v3d<real_t>::div_( const v3d<real_t> &v, const real_t a)
        {
            return mul_( real_t(1)/a, v );
        }
        
		template <>
		real_t v3d<real_t>::dot_( const v3d<real_t> &A, const v3d<real_t> &B ) throw() {
			return A.x * B.x + A.y * B.y + A.z * B.z;
		}
		
		template <>
		real_t v3d<real_t>:: norm2() const throw() {
			return x*x + y*y + z*z;
		}
		
#if YOCTO_ZTYPE != 'i'
		template <>
		real_t v3d<real_t>:: norm() const throw() {
			return math::Sqrt(x*x + y*y + z*z );
		}
#else
		template <>
		real_t v3d<real_t>:: norm() const throw() {
			return abs(x) + abs(y) + abs(z);
		}
#endif
		
		template <>
		v3d<real_t> v3d<real_t>::cross_( const v3d<real_t> &A, const v3d<real_t> &B ) throw() {
			return v3d<real_t>(
							   A.y * B.z - A.z * B.y,
							   A.z * B.x - A.x * B.z,
							   A.x * B.y - A.y * B.x
							   );
		}
		
#if YOCTO_ZTYPE != 'i'
		template <>
		void v3d<real_t>:: normalize() throw() {
			const real_t fac = math::Sqrt( real_t(1) / ( x*x + y*y + z*z ) );
			x *= fac;
			y *= fac;
			z *= fac;
			
		}
		
		template <>
		v3d<real_t>::v3d( Random::Uniform &ran ) throw() :
		x( 0 ),
		y( 0 ),
		z( 0 )
		{
			real_t       u     = real_t( ran() );
			const real_t theta = REAL_TWO_PI * real_t(ran());
			u += u;
			u -= REAL(1.0);
			const real_t rho = math::Sqrt( real_t(REAL(1.0) - u*u) );
			x =  rho * math::Cos( theta );
			y =  rho * math::Sin( theta );
			z = u;
		}
		
		template <>
		v3d<real_t>::v3d( Random::Uniform &ran, const real_t radius ) throw() :
		x( 0 ),
		y( 0 ),
		z( 0 )
		{
			real_t       u     = real_t( ran() );
			const real_t theta = REAL_TWO_PI *real_t(ran());
			u += u;
			u -= REAL(1.0);
			const real_t rho = radius * math::Sqrt( real_t(REAL(1.0) - u*u) );
			x =  rho * math::Cos( theta );
			y =  rho * math::Sin( theta );
			z =  radius * u;
		}
		
#endif
		
		template <>
		void v3d<real_t>:: output( std::ostream &os ) const {
			os << "[ " << x << " " << y << " " << z << " ]'";
		}
		
		template <>
		real_t & v3d<real_t>:: operator[]( size_t i ) throw()
		{
			assert(i<3);
			return *( &x + i);
		}
		
		template <>
		const real_t & v3d<real_t>:: operator[]( size_t i ) const throw()
		{
			assert(i<3);
			return *( &x + i);
		}
		
		
	}
	
}
