#include "yocto/utest/run.hpp"
#include "yocto/math/opt/minimize.hpp"
#include "yocto/math/opt/bracket.hpp"

#include <typeinfo>

using namespace yocto;
using namespace math;

template <typename T>
static inline T F( T x )
{
	return -Cos(x+T(0.23)) + T(7);
}

YOCTO_UNIT_TEST_IMPL(opt)
{
	numeric<float>::function func( cfunctor(F<float>) );
	triplet<float> x = { -1, 0 , 1};
	triplet<float> f = { func(x.a), 0, func(x.c) };
	if( ! bracket<float>::inside( func, x, f ) )
	{
		throw exception("Couldn't bracket !");
	}
	std::cerr << "x=" << x << std::endl;
	std::cerr << "f=" << f << std::endl;
	minimize<float> opt = { 1e-6f };
	opt( func,x,f);
	std::cerr << "min@" << x.b << " = " << f.b << std::endl;
	
}
YOCTO_UNIT_TEST_DONE()
