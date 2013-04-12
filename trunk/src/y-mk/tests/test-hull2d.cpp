#include "yocto/math/alg/delaunay.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/rand.hpp"

#include "yocto/ios/ocstream.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/string/conv.hpp"

using namespace yocto;
using namespace math;

typedef geom::v2d<double> V2;

YOCTO_UNIT_TEST_IMPL(tr2d)
{
    
    size_t  n=3;
    if( argc > 1 )
        n = strconv::to_size(argv[1],"n");
    
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
    
    vector<iTriangle> tr;
    delaunay<double>::build(tr,vec2);
    
    {
        ios::ocstream fp("triangles.dat",false);
        for(size_t i=1; i <= tr.size(); ++i)
        {
            fp("%g %g\n", vec2[ tr[i].p1 ].x, vec2[ tr[i].p1 ].y);
            fp("%g %g\n", vec2[ tr[i].p2 ].x, vec2[ tr[i].p2 ].y);
            fp("%g %g\n", vec2[ tr[i].p3 ].x, vec2[ tr[i].p3 ].y);
            fp("%g %g\n", vec2[ tr[i].p1 ].x, vec2[ tr[i].p1 ].y);
            fp("\n");
        }
    }
    
}
YOCTO_UNIT_TEST_DONE();


