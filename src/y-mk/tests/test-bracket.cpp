#include "yocto/utest/run.hpp"
#include "yocto/math/opt/bracket.hpp"

using namespace yocto;
using namespace math;

static
inline double func( double x )
{
    return 1.3+Cos(x+0.2);
}

YOCTO_UNIT_TEST_IMPL(bracket)
{
    numeric<double>::function F = cfunctor(func);
    triplet<double> x = { 0, 0.1 };
    triplet<double> f = { F(x.a), F(x.b) };
    
    bracket<double>::expand(F, x, f);
    
    
}
YOCTO_UNIT_TEST_DONE()

