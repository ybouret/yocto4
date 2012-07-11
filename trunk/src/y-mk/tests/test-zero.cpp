#include "yocto/utest/run.hpp"
#include "yocto/math/fcn/zfind.hpp"

#include <typeinfo>

using namespace yocto;
using namespace math;

template <typename T>
static inline T F( T x )
{
	return Sin(x) + T(0.5);
}

YOCTO_UNIT_TEST_IMPL(zero)
{
	zfind<float>             solve(1e-5f);
	numeric<float>::function func( cfunctor(F<float>) );
	const float z = solve( func, -numeric<float>::pi/2, numeric<float>::pi/2 );
	std::cerr << "F(" << z << ")=" << func(z) << std::endl;
}
YOCTO_UNIT_TEST_DONE()
