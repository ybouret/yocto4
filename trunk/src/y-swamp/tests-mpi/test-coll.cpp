#include "yocto/utest/run.hpp"
#include "yocto/swamp/mpi.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/auto-ptr.hpp"

using namespace yocto;
using namespace swamp;


double vproc( const double &x ) { return x; }

YOCTO_UNIT_TEST_IMPL(coll2D)
{
    
    mpi & MPI = mpi::init( &argc, &argv );
    const int rank = MPI.CommWorldRank;
    const int size = MPI.CommWorldSize;
    fields_setup<layout2D> F;
    Y_SWAMP_DECL_VAR(F, "A", array2D<double> );
    ghosts_setup<coord2D> G;
    if( size > 1 )
    {
        G.lower.count.y = 2;
        G.lower.peer.y  = MPI.CommWorldPrev();
        G.upper.count.y = 2;
        G.upper.peer.y  = MPI.CommWorldNext();
    }
    else {
        G.local.count.y = 2;
    }
    
    const layout2D full_layout( coord2D(1,1), coord2D(100,100) );
    const layout2D L = full_layout.split(rank,size);
    dataspace<layout2D> D( L, G, F );
    
    
    array2D<double> &A = D["A"].as< array2D<double> >();
    A.set_all( L, rank+1 );
    auto_ptr< array2D<double> > pA0;
    if( 0 == rank )
    {
        pA0.reset( new standalone< array2D<double> >(full_layout) );
    }
    
    D.prepare_ghosts();
    mpi::Requests requests( D.num_requests() );
    
    _mpi::init_exchange(MPI, D, requests);
    _mpi::wait_exchange(MPI, D, requests);
    
    _mpi::collect0(MPI, pA0.__get(), A, full_layout);
    
    if( 0 == rank )
        pA0->ppm( "full.ppm", "full", full_layout, vproc, NULL, 0, size );
    
    
}
YOCTO_UNIT_TEST_DONE()
