#include "yocto/utest/run.hpp"
#include "yocto/math/fcn/integrate.hpp"

using namespace yocto;
using namespace math;

template <typename T>
T F( T x ) 
{
	const T y = Cos(x);
	return y*y;
}

YOCTO_UNIT_TEST_IMPL(intg)
{
	{
		numeric<float>::function func( cfunctor( F<float>) );
		const float              ans  = integrate<float>( 0, numeric<float>::two_pi, func, 1e-5f )/numeric<float>::pi;
		std::cerr << "float:  " << ans << std::endl;
	}
	
	{
		numeric<double>::function func( cfunctor( F<double>) );
		const double              ans  = integrate<double>( 0, numeric<double>::two_pi, func, 1e-5 )/numeric<double>::pi;
		std::cerr << "double: " << ans << std::endl;
	}
}
YOCTO_UNIT_TEST_DONE()
