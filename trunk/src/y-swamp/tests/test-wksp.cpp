
#include "yocto/utest/run.hpp"

#include "yocto/swamp/workspace.hpp"
#include "yocto/swamp/array3d.hpp"
#include "yocto/code/rand.hpp"

#include <iostream>

using namespace yocto;
using namespace swamp;

static inline coord1D rand1()
{
    return 50 - 100 * alea<double>();
}

static inline coord2D rand2()
{
    return coord2D( rand1(), rand1() );
}

static inline coord3D rand3()
{
    return coord3D( rand1(), rand1(), rand1() );
}

YOCTO_UNIT_TEST_IMPL(wksp)
{
    {
        const layout1D        L( rand1()-2, rand1()+2 );
        ghosts_setup<coord1D> G;
        G.local.count = 1; // +1 and -1
        
        workspace<layout1D> W(L,G);
        
        typedef array1D<float> A1Df;
        W.create<A1Df>( "a1df", true );
        W.prepare_ghosts();
        
        for( size_t i=1; i <= W.local_ghosts_count(); ++i )
        {
            W.local_ghost(i).transfer( W.handles() );
        }
        
    }
    
    
    {
        const coord2D         lo(1,1);
        const coord2D         hi(10,10);
        const layout2D        L(lo,hi);
        ghosts_setup<coord2D> G;
        G.local.count.y = 3;
        
        G.lower.count.x = 2;
        G.upper.count.x = 1;
        
        workspace<layout2D>   W(L,G);
        typedef geom::v2d<double> vtx;
        typedef array2D<vtx>      A2Dv; 
        A2Dv &A1 = W.create<A2Dv>( "a2dv" );
        A2Dv &A2 = W["a2dv"].as<A2Dv>();
        if( &A1 != &A2 )
            throw exception("arrays mismatch!");
        typedef array2D<double> A2D;
        (void)W.create<A2D>( "a2d" );
        W.prepare_ghosts();
        for( size_t i=1; i <= W.local_ghosts_count(); ++i )
        {
            W.local_ghost(i).transfer( W.handles() );
        }
    }
    
    
}
YOCTO_UNIT_TEST_DONE()