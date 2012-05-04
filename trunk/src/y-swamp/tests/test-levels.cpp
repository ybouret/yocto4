#include "yocto/utest/run.hpp"

#include "yocto/swamp/level-set.hpp"
#include "yocto/swamp/workspace.hpp"

using namespace yocto;
using namespace swamp;


YOCTO_UNIT_TEST_IMPL(levels)
{
    
    level_set<double> levels;
    levels.add(0,0.1);
    levels.add(1,0.2);
    
    for( size_t i=1; i <= levels.size(); ++i )
    {
        std::cerr << levels[i].value << std::endl;
    }
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/swamp/contour2d.hpp"
#include "yocto/swamp/workspace.hpp"
#include "yocto/swamp/rmesh.hpp"

typedef array1D<double>          Array1D;
typedef array2D<double>          Array2D;
typedef region2D<double>::type   Region2D;
typedef vertex2D<double>::type   V2D;

#include "yocto/ios/ocstream.hpp"

namespace 
{
    static double f2d( double x, double y )
    {
        
        const double r = sqrt(3*x*x+y*y);
        const double yc = y-0.5;
        const double r2 = sqrt(x*x*+yc*yc);
        return cos(3*r-2*r2);
        
    }
    
    static double vproc( const double &x ) { return x; }
    
    struct handler
    {
        void init( const level_set<double> &levels )
        {
            for( size_t i=1; i <= levels.size(); ++i )
            {
                const string filename = vformat( "l%d.dat", levels[i].key );
                ios::ocstream fp( filename, false );
            }
        }
        
        void process( const V2D &v1, const V2D &v2, const level<double> &l )
        {
            const string filename = vformat( "l%d.dat", l.key );
            ios::ocstream fp( filename, true );
            fp("%g %g\n", v1.x, v1.y);
            fp("%g %g\n", v2.x, v2.y);
        }
    };
    
}

YOCTO_UNIT_TEST_IMPL(C2D)
{
    
    fields_setup<layout2D> F;
    Y_SWAMP_DECL_VAR(F,"A", Array2D );
    const layout2D         L( coord2D(1,1), coord2D(20,20) );
    const ghosts_setup<coord2D> no_ghosts;
    workspace<layout2D,double,rmesh> W( L, no_ghosts, F );
    const region2D<double>::type     R( V2D(-1,-1), V2D(1,1) );
    W.mesh.regular_map_to(R,L);
    const Array1D &X = W.mesh.X();
    const Array1D &Y = W.mesh.Y();
    
    Array2D       &A = W["A"].as<Array2D>();
    
    for( unit_t j= A.lower.y; j <= A.upper.y; ++j )
    {
        const double y = Y[j];
        for( unit_t i=A.lower.x; i <= A.upper.x; ++i )
        {
            const double x = X[i];
            A[j][i] = f2d(x,y);
        }
    }
    
    A.ppm("c2d.ppm", "c3d", A, vproc,NULL,-1,1);
    
    level_set<double> levels;
    levels.add(1,-0.1);
    levels.add(2, 0.1);
    
    handler H;
    H.init(levels);
    contour2D<double>::callback cb( &H, & handler::process );
    contour2D<double>::compute(A, X, Y, A, levels, cb);
    
}
YOCTO_UNIT_TEST_DONE()
