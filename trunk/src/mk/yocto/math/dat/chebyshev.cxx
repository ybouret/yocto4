#include "yocto/math/dat/chebyshev.hpp"
#include "yocto/math/ztype.hpp"

namespace yocto {

	namespace math {



		template<>
		chebyshev<real_t>:: chebyshev( real_t a, real_t b, size_t n, numeric<real_t>::function  &func) :
			a_( a ),
			b_( b ),
			bma_( b-a ),
			bpa_( b+a ),
			wksp_( n, 0 ),
			coef_( wksp_(0) ),
			hc0_(0)
		{
			assert(a<b || a>b);
			assert(n>0);
			const real_t bma = REAL(0.5) * bma_;
			const real_t bpa = REAL(0.5) * bpa_;
			vector<real_t> tmp(n,0);
			real_t        *f = tmp(0);
			for( size_t k=0; k < n; ++k )
			{
				const real_t y = Cos( numeric<real_t>::pi * real_t(k+0.5)/ real_t(n));
				f[k] = func(y*bma + bpa);
			}
			const real_t  fac = real_t(2.0/n);
			for( size_t j=0;j<n;++j)
			{
				real_t sum = 0;
				for( size_t k=0; k <n; ++k )
				{
					sum += f[k] * Cos( real_t(j) * numeric<real_t>::pi * real_t(k-0.5) / real_t(n) );
				}
				coef_[j] = sum * fac;
			}
			*(real_t *)&hc0_ = coef_[0] * REAL(0.5);
		}

		template <>
		chebyshev<real_t>:: ~chebyshev() throw()
		{

		}

		template<>
		size_t chebyshev<real_t>:: size() const throw()
		{
			return wksp_.size();
		}

		template<>
		real_t chebyshev<real_t>:: get( size_t m, real_t x ) const throw()
		{
			assert(m>0);
			assert(m<=size());
			real_t d = 0, dd = 0;

			const real_t y  = ( (x+x) - bpa_ ) / bma_;
			const real_t y2 = y+y;

			for( size_t j=m-1;j>0;--j)
			{
				const real_t old_d = d;
				d  = y2 *d - dd + coef_[j];
				dd = old_d;
			}

			return y*d - dd + hc0_;
		}

	}

}
