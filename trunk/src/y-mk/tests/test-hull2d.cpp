#include "yocto/math/algo2d/delaunay.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/rand.hpp"

#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace math;

typedef geom::v2d<double> vtx;

YOCTO_UNIT_TEST_IMPL(tr2d)
{
    const size_t n = 10;
    vector<vtx> vertices;
    for(size_t i=n;i>0;--i)
    {
        const vtx v( 10 * ( alea<double>() - 0.5 ), 10 * ( alea<double>() - 0.5) );
        vertices.push_back(v);
    }
    
    {
        ios::ocstream fp("vtx.dat",false);
        for( size_t i=1; i <= n; ++i )
            fp("%g %g\n", vertices[i].x, vertices[i].y);
    }
    
    iTriangle::List trlist;
    
    algo2d::delaunay<double>::build(trlist, vertices);
    
}
YOCTO_UNIT_TEST_DONE();


