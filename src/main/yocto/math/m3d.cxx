
#include "yocto/math/m3d.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/code/bswap.hpp"

#include <iostream>
#include <cstring>

namespace yocto {

	namespace math {

		
		template<>
		m3d<real_t>:: m3d(const real_t diag) throw() :
				ex(diag,0,0),
				ey(0,diag,0),
				ez(0,0,diag)
		{
		}

		template <>
		m3d<real_t>:: ~m3d() throw() {
		}


		template<>
		m3d<real_t>:: m3d( const v3d<real_t> &vx, const v3d<real_t> &vy, const v3d<real_t> &vz ) throw() :
				ex(vx),
				ey(vy),
				ez(vz)
		{

		}

		template<>
		m3d<real_t>:: m3d( const m3d<real_t> &m ) throw() :
				ex( m.ex ),
				ey( m.ey ),
				ez( m.ez )
		{
		}



		template<>
		m3d<real_t> & m3d<real_t>::operator=( const m3d<real_t> &m ) throw()  {
			ex = m.ex;
			ey = m.ey;
			ez = m.ez;
			return *this;
		}


		//----------------------------------------------------------------------
		// +
		//----------------------------------------------------------------------
		template <>
		m3d<real_t> & m3d<real_t>:: operator+=( const m3d<real_t> &m ) throw() {
			ex += m.ex;
			ey += m.ey;
			ez += m.ez;
			return *this;
		}

		template <>
		m3d<real_t> m3d<real_t>:: operator+() const throw() {
			return m3d<real_t>( *this );
		}

		template <>
		m3d<real_t> m3d<real_t>:: operator+(const m3d<real_t> &m ) const throw() {
			return m3d<real_t>( ex + m.ex, ey + m.ey, ez + m.ez );
		}

		//----------------------------------------------------------------------
		// -
		//----------------------------------------------------------------------
		template <>
		m3d<real_t> & m3d<real_t>:: operator-=( const m3d<real_t> &m ) throw() {
			ex -= m.ex;
			ey -= m.ey;
			return *this;
		}

		template <>
		m3d<real_t> m3d<real_t>:: operator-() const throw() {
			return m3d<real_t>( -ex, -ey, -ez );
		}

		template <>
		m3d<real_t> m3d<real_t>:: operator-(const m3d<real_t> &m ) const throw() {
			return m3d<real_t>( ex - m.ex, ey - m.ey, ez - m.ez );
		}

		template <>
		m3d<real_t> & m3d<real_t>:: operator *= ( const real_t a ) throw() {
			ex *= a;
			ey *= a;
			ez *= a;
			return *this;
		}



		template <>
		m3d<real_t>  operator*(const real_t a, const m3d<real_t> &m) throw() {
			m3d<real_t> ans( m );
			return (ans *= a);
		}

		template <>
		m3d<real_t>  operator*(const m3d<real_t> &a, const m3d<real_t> &b) throw() {
			const v3d<real_t> vx( a.ex.x * b.ex.x + a.ey.x * b.ex.y + a.ez.x * b.ex.z,
			                      a.ex.y * b.ex.x + a.ey.y * b.ex.y + a.ez.y * b.ex.z,
			                      a.ex.z * b.ex.x + a.ey.z * b.ex.y + a.ez.z * b.ex.z );
			const v3d<real_t> vy( a.ex.x * b.ey.x + a.ey.x * b.ey.y + a.ez.x * b.ey.z,
			                      a.ex.y * b.ey.x + a.ey.y * b.ey.y + a.ez.y * b.ey.z,
			                      a.ex.z * b.ey.x + a.ey.z * b.ey.y + a.ez.z * b.ey.z );
			const v3d<real_t> vz( a.ex.x * b.ez.x + a.ey.x * b.ez.y + a.ez.x * b.ez.z,
			                      a.ex.y * b.ez.x + a.ey.y * b.ez.y + a.ez.y * b.ez.z,
			                      a.ex.z * b.ez.x + a.ey.z * b.ez.y + a.ez.z * b.ez.z );
			return m3d<real_t>(vx,vy,vz);
		}

		template <>
		m3d<real_t> & m3d<real_t>:: operator *= ( const m3d<real_t> &m ) throw() {
			return ( *this = ( (*this) *  m ) );
		}


	#define ma ex.x
	#define mb ey.x
	#define mc ez.x

	#define md ex.y
	#define me ey.y
	#define mf ez.y


	#define mg ex.z
	#define mh ey.z
	#define mi ez.z



		template <>
		real_t m3d<real_t>:: det(void) const throw() {
			return ( ma * me * mi + md * mh * mc + mg * mb * mf ) - ( mg * me * mc + ma * mh * mf + md * mb * mi );
		}

		template <>
		void m3d<real_t>:: ortho(void) throw() {
			ex.normalize();
			ey -= (ex*ey) * ex;
			ey.normalize();
			ez  = ex ^ ey;
		}

		template <>
		void m3d<real_t>:: transpose(void) throw() {
			cswap( ex.y, ey.x );
			cswap( ex.z, ez.x );
			cswap( ey.z, ez.y );
		}

		template <>
		void  m3d<real_t>:: inverse(void) throw() {
			const real_t d = this->det();
			const real_t fac = real_t(1)/d;

			//-- cofactors
			const real_t xx =  (ey.y * ez.z - ey.z * ez.y);
			const real_t xy = -(ey.x * ez.z - ey.z * ez.x);
			const real_t xz =  (ey.x * ez.y - ey.y * ez.x);

			const real_t yx = -(ex.y * ez.z - ex.z * ez.y);
			const real_t yy =  (ex.x * ez.z - ex.z * ez.x);
			const real_t yz = -(ex.x * ez.y - ex.y * ez.x);

			const real_t zx =  (ex.y * ey.z - ex.z * ey.y);
			const real_t zy = -(ex.x * ey.z - ex.z * ey.x);
			const real_t zz =  (ex.x * ey.y - ex.y * ey.x);

			memset(&ex.x,0,3*sizeof(v3d<real_t>));
			ex.x = xx * fac;
			ex.y = yx * fac;
			ex.z = zx * fac;

			ey.x = xy * fac;
			ey.y = yy * fac;
			ey.z = zy * fac;

			ez.x = xz * fac;
			ez.y = yz * fac;
			ez.z = zz * fac;
		}

		template <>
		std::ostream &m3d<real_t>::to_scilab( std::ostream &os ) const
		{
			return os << "[ " << ex.x << " " << ey.x << " " << ez.x << " ; "
			       /* */      << ex.y << " " << ey.y << " " << ez.y << " ; "
			       /* */      << ex.z << " " << ey.z << " " << ez.z << " ]";
		}

		template <>
		void m3d<real_t>:: mul( v3d<real_t> &target, const v3d<real_t> &source ) const throw()
		{
			assert( &source != &target );
			target.x = ex.x * source.x + ey.x * source.y + ez.x * source.z;
			target.y = ex.y * source.x + ey.y * source.y + ez.y * source.z;
			target.z = ex.z * source.x + ey.z * source.y + ez.z * source.z;

		}
		
		template <>
		void m3d<real_t>:: mul( v3d<real_t> &source ) const throw()
		{
			const real_t target_x = ex.x * source.x + ey.x * source.y + ez.x * source.z;
			const real_t target_y = ex.y * source.x + ey.y * source.y + ez.y * source.z;
			const real_t target_z = ex.z * source.x + ey.z * source.y + ez.z * source.z;
			source.x = target_x;
			source.y = target_y;
			source.z = target_z;
		}

        template<>
        real_t & m3d<real_t>:: operator()( size_t i, size_t j ) throw()
        {
            assert(i>=1);
            assert(i<=3);
            assert(j>=1);
            assert(j<=3);
            real_t *a = &ex.x;
            --i;
            --j;
            return a[i*3+j];
        }
        
        template<>
        const real_t & m3d<real_t>:: operator()( size_t i, size_t j ) const throw()
        {
            assert(i>=1);
            assert(i<=3);
            assert(j>=1);
            assert(j<=3);
            const real_t *a = &ex.x;
            --i;
            --j;
            return a[i*3+j];
        }


              
        
	}
}
