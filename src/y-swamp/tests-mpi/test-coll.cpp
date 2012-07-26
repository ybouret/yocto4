#include "yocto/utest/run.hpp"

#include "yocto/swamp/common.hpp"
#include "yocto/swamp/mpi.hpp"

#include "yocto/ios/ocstream.hpp"
#include "yocto/auto-ptr.hpp"

using namespace yocto;
using namespace swamp;


static inline double vproc( const double &x ) { return x; }

YOCTO_UNIT_TEST_IMPL(coll2D)
{
    
    mpi & MPI = mpi::init( &argc, &argv );
    const int rank = MPI.CommWorldRank;
    const int size = MPI.CommWorldSize;
    fields_setup<layout2D> F;
    Y_SWAMP_DECL_VAR(F, "A", array2D<double> );
    Y_SWAMP_DECL_AUX(F, "B", array2D<double> );
    
    ghosts_setup<coord2D> G;
    if( size > 1 )
    {
        G.lower.count.y = 2;
        G.lower.peer.y  = MPI.CommWorldPrev();
        G.upper.count.y = 2;
        G.upper.peer.y  = MPI.CommWorldNext();
    }
    else 
    {
        G.local.count.y = 2;
    }
    
    const layout2D full_layout( coord2D(1,1), coord2D(100,100) );
    const layout2D L = full_layout.split(rank,size);
    dataspace<layout2D> D( L, G, F );
    
    
    array2D<double> &A = D["A"].as< array2D<double> >();
    array2D<double> &B = D["B"].as< array2D<double> >();

    A.set_all( L, rank+1    );
    B.set_all( L, size-rank );
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
        pA0->ppm( "fullA.ppm", "fullA", full_layout, vproc, 0, 0, size );
    MPI.Barrier(MPI_COMM_WORLD);
    
    _mpi::synchronize1(B, MPI, D, requests);
    _mpi::collect0(MPI, pA0.__get(), B, full_layout);

    if( 0 == rank )
        pA0->ppm( "fullB.ppm", "fullB", full_layout, vproc, 0, 0, size );

    
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/swamp/vtk-writer.hpp"

YOCTO_UNIT_TEST_IMPL(coll3D)
{
    
    typedef workspace<layout3D,double,rmesh> W3;
    
    mpi & MPI = mpi::init( &argc, &argv );
    const int rank = MPI.CommWorldRank;
    const int size = MPI.CommWorldSize;
    fields_setup<layout3D> F;
    Y_SWAMP_DECL_VAR(F, "A", array3D<double> );
    ghosts_setup<coord3D> G;
    if( size > 1 )
    {
        G.lower.count.z = 2;
        G.lower.peer.z  = MPI.CommWorldPrev();
        G.upper.count.z = 2;
        G.upper.peer.z  = MPI.CommWorldNext();
    }
    else 
    {
        G.local.count.z = 2;
    }
    
    const layout3D full_layout( coord3D(1,1,1), coord3D(10,10,100) );
    const layout3D L = full_layout.split(rank,size);
    W3             D( L, G, F );
    typedef vertex3D<double>::type V3D;
    const   region3D<double>::type R( V3D(-1,-1,-1), V3D(1,1,1) );
    
    D.mesh.regular_map_to(R, full_layout);
    
    
    array3D<double> &A = D["A"].as< array3D<double> >();
    A.set_all( L, rank+1 );
    auto_ptr< W3 > pW;
    array3D<double> *pA0 = 0;
    if( 0 == rank )
    {
        const ghosts_setup<coord3D> no_ghosts;
        pW.reset( new W3(full_layout,no_ghosts,F) );
        pA0 = &  (*pW)["A"].as< array3D<double> >();
        pW->mesh.regular_map_to(R,full_layout);
    }
    
    D.prepare_ghosts();
    mpi::Requests requests( D.num_requests() );
    
    _mpi::init_exchange(MPI, D, requests);
    _mpi::wait_exchange(MPI, D, requests);
    
    _mpi::collect0(MPI, pA0, A, full_layout);
    
    variables var;
    var << "A" ;
    
    vtk_writer vtk;
    if( 0 == rank )
    {
        vtk.save("full.vtk", "full3D", *pW, var, full_layout);
    }
   
	const string fn = vformat("part%d.vtk",rank); 
    vtk.save( fn, fn, D, var, L);
    
    
}
YOCTO_UNIT_TEST_DONE()



