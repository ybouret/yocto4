
#include "yocto/geom/color.hpp"
#include "yocto/math/ztype.hpp"

#include "yocto/code/utils.hpp"

namespace yocto {

	using namespace math;
	
	namespace color {

		template <>
		rgba<real_t>:: rgba() throw() : r(0), g(0), b(0), a(1) {
		}

		template <>
		rgba<real_t>:: rgba( const real_t R, const real_t G, const real_t B) throw() : r(R), g(G), b(B), a(1) {
		}

		template <>
		rgba<real_t>:: rgba(const real_t R, const real_t G, const real_t B, const real_t A) throw() : r(R), g(G), b(B), a(A) {

		}

		template <>
		rgba<real_t>:: rgba( const rgba<real_t> &c ) throw() :
				r( c.r ),
				g( c.g ),
				b( c.b ),
				a( c.a )
		{

		}

		template <>
		rgba<real_t>:: rgba( const rgba<real_t> &c, const real_t A ) throw() :
				r( c.r ),
				g( c.g ),
				b( c.b ),
				a( A )
		{

		}

		template <>
		rgba<real_t> rgba<real_t>::ramp( real_t v, const real_t vmin, const real_t vmax) throw() {
			rgba<real_t> c(1,1,1,1);
			if (v < vmin)
				v = vmin;
			if (v > vmax)
				v = vmax;
			const real_t dv = vmax - vmin;

			if (v < (vmin + REAL(0.25) * dv)) {
				c.r = REAL(0.0);
				c.g = REAL(4.0) * (v - vmin) / dv;
			} else if (v < (vmin + REAL(0.5) * dv)) {
				c.r = REAL(0.0);
				c.b = REAL(1.0) + REAL(4.0) * (vmin + REAL(0.25) * dv - v) / dv;
			} else if (v < (vmin + REAL(0.75) * dv)) {
				c.r = REAL(4.0) * (v - vmin - REAL(0.5) * dv) / dv;
				c.b = REAL(0.0);
			} else {
				c.g = REAL(1.0) + REAL(4.0) * (vmin + REAL(0.75) * dv - v) / dv;
				c.b = REAL(0.0);
			}

			return c;
		}
		
		
		template <>
		rgba32 rgba32:: ramp( const rgba32 colors[256], real_t v, real_t vmin, real_t vmax) throw()
		{
			if( v <= vmin )
				return colors[0];
			
			if( v >= vmax )
				return colors[255];
			
			return colors[ clamp<int>( 0, int( REAL(256.0) * (v - vmin ) /( vmax - vmin) ), 255 ) ];
		}

	}



}
