#include "yocto/math/stat/tests.hpp"
#include "yocto/math/ztype.hpp"

namespace yocto {

	namespace math {

		template <>
		real_t probks( real_t x ) throw()
		{
			static const real_t EPS1 = std::pow( REAL(10.0), -real_t(numeric<real_t>::dig/2) );
			static const real_t EPS2 = std::pow( REAL(10.0), -real_t(numeric<real_t>::dig)   );

			//std::cout << "probks(" << x << ")" << std::endl;

			const real_t a2  = - REAL(2.0) * x * x;
			real_t       fac = REAL(2.0);
			real_t       sum = 0;
			real_t       termbf = 0;
			for( size_t j=1;j<=128;++j)
			{
				const real_t term = fac * std::exp( a2 * j * j );
				sum += term;
				if( std::fabs( term ) <= EPS1 * termbf || std::fabs(term) <= EPS2 * sum )
					return sum;
				fac = -fac;
				termbf = std::fabs( term );
			}
			return 1;
		}

	}

}

