#include "yocto/utest/run.hpp"

#include "yocto/swamp/common.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/swamp/delaunay.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace swamp;

typedef vertex2D<double>::type vertex;

YOCTO_UNIT_TEST_IMPL(delaunay)
{
    vector<vertex> vertices;
    for( size_t i=1; i <= 3; ++i )
    {
        const vertex v( 10 * (0.5-alea<double>()), 10 * (0.5-alea<double>()) );
        vertices.push_back(v);
    }
    
    delaunay<double> D;
    D.build(vertices);
    const delaunay<double>::triangles &tr_list = D();
    std::cerr << "#tr=" << tr_list.size << std::endl;
    for(  const delaunay<double>::triangle *tr = tr_list.head; tr; tr = tr->next )
    {
        std::cerr << vertices[tr->i0] << "->" <<  vertices[tr->i1] << "->" <<  vertices[tr->i2] << std::endl;
        std::cerr << "\tcenter@" << tr->center << " | radius=" << tr->radius << std::endl;
    }
    
    {
        ios::ocstream fp( "vtx.dat", false );
        for( size_t i=1; i <= vertices.size(); ++i )
        {
            fp("%g %g\n", vertices[i].x, vertices[i].y );
        }
    }
    
    {
        ios::ocstream fp( "ctx.dat", false );
        for(  const delaunay<double>::triangle *tr = tr_list.head; tr; tr = tr->next )
        {
            fp("%g %g\n", tr->center.x, tr->center.y);
        }
    }
    
    {
        ios::ocstream fp( "crd.dat", false );
        for(  const delaunay<double>::triangle *tr = tr_list.head; tr; tr = tr->next )
        {
            const vertex &v = tr->center;
            const double  r = tr->radius;
            for( size_t i=0; i < 20; ++i)
            {
                const double alpha = (i*6.28) / 20;
                fp("%g %g\n", v.x + r * sin(alpha), v.y + r * cos(alpha) ); 
            }
            
        }
    }
    
}
YOCTO_UNIT_TEST_DONE()
