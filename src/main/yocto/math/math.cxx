#include "yocto/math/ztype.hpp"
#include "yocto/math/types.hpp"

namespace yocto 
{
	namespace math
	{
	
		template <> const real_t numeric<real_t>::epsilon      = REAL_EPSILON;
		template <> const real_t numeric<real_t>::minimum      = REAL_MIN;
		template <> const real_t numeric<real_t>::maximum      = REAL_MAX;
		template <> const real_t numeric<real_t>::pi           = REAL_PI;
		template <> const real_t numeric<real_t>::two_pi       = REAL_TWO_PI;
        template <> const real_t numeric<real_t>::half_pi      = REAL_PI*REAL(0.5);
		template <> const real_t numeric<real_t>::sqrt_epsilon = Sqrt( REAL_EPSILON );
		template <> const real_t numeric<real_t>::min_exp      = REAL_MIN_EXP;
		template <> const real_t numeric<real_t>::max_exp      = REAL_MAX_EXP;
		template <> const size_t numeric<real_t>::dig          = REAL_DIG;
		template <> const real_t numeric<real_t>::ftol         = Pow( REAL(10.0), -real_t(dig) );
        template <> const real_t numeric<real_t>::sqrt_ftol    = Pow( REAL(10.0), -real_t(dig/2+1) );
		template <> const real_t numeric<real_t>::min_10_exp   = REAL_MIN_10_EXP;
		template <> const real_t numeric<real_t>::max_10_exp   = REAL_MAX_10_EXP;
		template <> const real_t numeric<real_t>::tiny         = Pow( REAL(10.0), numeric<real_t>::min_10_exp );
		template <> const real_t numeric<real_t>::huge         = Pow( REAL(10.0), numeric<real_t>::max_10_exp );
		template <> const real_t numeric<real_t>::sqrt_tiny    = Pow( REAL(10.0), Floor(numeric<real_t>::min_10_exp/2) );

	}
	
}
