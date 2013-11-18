#include "yocto/math/dat/interpolate.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/exceptions.hpp"

#include <cerrno>

namespace yocto {

	namespace math {

		template <> interpolate<real_t>::  interpolate() throw() : wksp_() {}
		template <> interpolate<real_t>:: ~interpolate() throw() {}
		template <> interpolate<real_t>::  interpolate(const size_t n) : wksp_(n+n,true) {}


		template <>
		void interpolate<real_t>:: acquire( const size_t nvar ) {
			wksp_.make( nvar+nvar,0 );
		}

		template <>
		void interpolate<real_t>:: release() throw() {
			wksp_.release();
		}

		template <>
		real_t interpolate<real_t>:: polint( const real_t x, const array<real_t> &xa, const array<real_t> &ya, real_t &dy ) const {
			assert(xa.size()==ya.size());
			assert(xa.size()>0);
			assert(wksp_.size()>=xa.size()*2);

			const size_t n = xa.size();
			real_t *c = wksp_(-1);
			real_t *d = c+n;

			real_t  ds = Fabs( x - xa[1] );
			size_t  ns = 1;

			for( register size_t i = n; i>0; --i ) {
				const real_t l = Fabs( x - xa[i] );
				if( l < ds ) {
					ds = l;
					ns = i;
				}
				c[i] = ya[i];
				d[i] = ya[i];
			}

			real_t y = ( dy = ya[ns--] );

			for(register size_t m=1;m<n;++m) {
				for(register size_t i=1;i<=n-m;++i) {
					const real_t ho  = xa[i]  -x;
					const real_t hp  = xa[i+m]-x;
					const real_t w   = c[i+1]-d[i];
					real_t       den = ho - hp;
					if( Fabs( den ) <= REAL_MIN )
						throw libc::exception( EDOM, "interpolate::polint(div by zero)");
					den  = w/den;
					d[i] = hp*den;
					c[i] = ho*den;
				}
				y += ( dy=( (ns+ns) < (n-m) ? c[ns+1] : d[ns--]));
			}

			return y;
		}

		template <>
		real_t interpolate<real_t>:: ratint( const real_t x, const array<real_t> &xa, const array<real_t> &ya, real_t &dy ) const {
			assert(xa.size()==ya.size());
			assert(xa.size()>0);
			assert(wksp_.size()>=xa.size()*2);

			const size_t n = xa.size();
			real_t *c = wksp_(-1);
			real_t *d = c+n;

			real_t  ds = Fabs( x - xa[1] );
			size_t  ns = 1;

			for( size_t i=n; i>0; --i ) {
				const real_t l = Fabs( x - xa[i] );
				if( l <= REAL_MIN ) {
					dy = 0;
					return ya[i];
				} else {

					if( l < ds ) {
						ds = l;
						ns = i;
					}

				}
				c[i] = ya[i];
				d[i] = ya[i];
			}

			real_t y = ( dy = ya[ns--] );

			for(register size_t m=1;m<n;m++) {
				for(register size_t i=1;i<=n-m;i++) {
					const real_t w  = c[i+1]-d[i];
					const real_t h  = xa[i+m]-x;
					const real_t t  = (xa[i]-x) * d[i]/h;
					real_t       dd = t-c[i+1];
					if( Fabs(dd) <= REAL_MIN)
						throw libc::exception( EDOM, "interpolate::ratint(div by zero)");
					dd=w/dd;
					d[i]=c[i+1]*dd;
					c[i]=t*dd;
				}
				y += (dy=( (ns+ns) < (n-m) ? c[ns+1] : d[ns--]));
			}

			return y;
		}

	}
}
