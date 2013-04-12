#include "yocto/math/alg/delaunay.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/rand.hpp"

#include "yocto/ios/ocstream.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;
using namespace math;

typedef geom::v2d<double> V2;

YOCTO_UNIT_TEST_IMPL(tr2d)
{
    
    const int  n=3;
    vector<V2> vec2(n,as_capacity);
    for(size_t i=1; i<=n; ++i )
    {
        const V2 tmp( 10.0 * ( 0.5 - alea<double>()), 10.0 * ( 0.5 - alea<double>()) );
        vec2.push_back(tmp);
    }
    
    {
        ios::ocstream fp("points.dat",false);
        for( size_t i=1; i <= vec2.size(); ++i )
        {
            fp("%g %g\n", vec2[i].x, vec2[i].y);
        }
    }
    
    delaunay<double>::build(vec2);
    
}
YOCTO_UNIT_TEST_DONE();


