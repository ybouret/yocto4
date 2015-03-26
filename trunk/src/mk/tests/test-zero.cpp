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
    {
        zfind<float>             solve(1e-5f);
        numeric<float>::function func( cfunctor(F<float>) );
        const float z = solve( func, -numeric<float>::pi/2, numeric<float>::pi/2 );
        std::cerr << "F(" << z << ")=" << func(z) << std::endl;
    }
    
    {
        zfind<double> solve(1e-5);
        numeric<double>::function func( cfunctor(F<double>) );
        numeric<double>::function f2 = func;
        
        triplet<double> x = { 0.0, 0, 0.5 };
        triplet<double> f = { 0, 0, 0 };
        if( !solve.lookup(func, x, f))
        {
            std::cerr << "Couldn't find a zero" << std::endl;
        }
        else
        {
            std::cerr << "found x=" << x << " and f=" << f << std::endl;
            const double z = solve.run(func, x, f);
            std::cerr << "F(" << z << ")=" << func(z) << "/" << f.b << std::endl;
        }
    }
    
}
YOCTO_UNIT_TEST_DONE()
