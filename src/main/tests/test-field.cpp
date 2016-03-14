#include "yocto/parallel/field.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/string.hpp"
#include "support.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(field)
{
    parallel::field1D<float> F1( parallel::patch1D(-10,12) );
    for(unit_t x=F1.lower;x<=F1.upper;++x)
    {
        F1[x] = alea<float>();
    }

    const parallel::coord2D   c2min(-5,-8), c2max(15,10);
    const parallel::patch2D   p2(c2min,c2max);
    parallel::field2D<double> F2d(p2);
    for(unit_t y=F2d.lower.y;y<=F2d.upper.y;++y)
    {
        for(unit_t x=F2d.lower.x;x<=F2d.upper.x;++x)
        {
            F2d[y][x] = alea<double>();
        }
    }

    parallel::field2D<string> F2s(p2);
    for(unit_t y=F2s.lower.y;y<=F2s.upper.y;++y)
    {
        for(unit_t x=F2s.lower.x;x<=F2s.upper.x;++x)
        {
            F2s[y][x] = gen<string>::get();
        }
    }


}
YOCTO_UNIT_TEST_DONE()
