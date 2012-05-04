
#include "yocto/utest/run.hpp"

#include "yocto/swamp/dataspace.hpp"
#include "yocto/swamp/array3d.hpp"
#include "yocto/code/rand.hpp"


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

static inline double vproc( const double &x )
{
    return x;
}

YOCTO_UNIT_TEST_IMPL(data)
{

    {
        fields_setup<layout1D> F;
        const layout1D        L( rand1()-2, rand1()+2 );
        ghosts_setup<coord1D> G;
        G.local.count = 1; // +1 and -1
        
        dataspace<layout1D> W(L,G,F);
        
        typedef array1D<float> A1Df;
        W.create<A1Df>( "a1df", true );
        W.prepare_ghosts();
        
        for( size_t i=1; i <= W.local_ghosts_count(); ++i )
        {
            W.local_ghost(i).transfer( W.handles() );
        }
        
    }
    
    
    {
        fields_setup<layout2D> F;

        const coord2D         lo(1,1);
        const coord2D         hi(10,10);
        const layout2D        L(lo,hi);
        ghosts_setup<coord2D> G;
        G.local.count.y = 3;
        
        G.lower.count.x = 2;
        G.upper.count.x = 1;
        
        dataspace<layout2D>       W(L,G,F);
        typedef geom::v2d<double> vtx;
        typedef array2D<vtx>      A2Dv; 
        A2Dv &A1 = W.create<A2Dv>( "a2dv" );
        A2Dv &A2 = W["a2dv"].as<A2Dv>();
        if( &A1 != &A2 )
            throw exception("arrays mismatch!");
        typedef array2D<double> A2D;
        A2D &A = W.create<A2D>( "a2d" );
        W.prepare_ghosts();
        for( size_t i=1; i <= W.local_ghosts_count(); ++i )
        {
            W.local_ghost(i).transfer( W.handles() );
        }
        
        std::cerr << "0: " << W.outline    << std::endl;
        std::cerr << "1: " << W.__layout() << std::endl;
        std::cerr << "2: " << W.sync       << std::endl;
        A.set_all( W.outline,    0 );
        A.set_all( W.__layout(), 1 );
        A.set_all( W.sync,       2 );
        
        A.ppm("ini2.ppm", "ghosts-and-sync", A, vproc, NULL, 0, 2);
        
        std::cerr << "Test Communication 2D" << std::endl;
        for( size_t i=1; i <= W.local_ghosts_count(); ++i )
        {
            W.local_ghost(i).transfer( W.handles() );
        }
        
        for( size_t i=1; i <= W.async_ghosts_count(); ++i )
        {
            async_ghosts &g = W.async_ghost(i);
            g.store_inner( W.handles() );
            memcpy( g.outer_buf, g.inner_buf, g.length );
            g.query_outer( W.handles() );
        }
        
        A.ppm("end2.ppm", "ghosts-and-sync", A, vproc, NULL, 0, 2);
    }

    
    {
        fields_setup<layout3D> F;

        const coord3D  lo(1,1,1);
        const coord3D  hi(10,15,20);
        const layout3D L( lo, hi );
        ghosts_setup<coord3D> G;
        G.local.count.x = 2;   // two local ghosts along x
        G.lower.count.y = 1;   // one async ghost along -y
        G.upper.count.y = 3;   // 3   async ghost along  y
        G.lower.count.z = 4;
        G.upper.count.z = 5;
        
        dataspace<layout3D>   W(L,G,F);
        
        std::cerr << "Test Communication 3D" << std::endl;
        for( size_t i=1; i <= W.local_ghosts_count(); ++i )
        {
            W.local_ghost(i).transfer( W.handles() );
        }
        
        for( size_t i=1; i <= W.async_ghosts_count(); ++i )
        {
            async_ghosts &g = W.async_ghost(i);
            g.store_inner( W.handles() );
            memcpy( g.outer_buf, g.inner_buf, g.length );
            g.query_outer( W.handles() );
        }

    }
    
    
}
YOCTO_UNIT_TEST_DONE()