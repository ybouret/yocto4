#include "yocto/utest/run.hpp"
#include "yocto/swamp/mpi.hpp"
#include "yocto/swamp/array3d.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace swamp;


YOCTO_UNIT_TEST_IMPL(d1)
{
    
    mpi & MPI = mpi::init( &argc, &argv );
    const int size = MPI.CommWorldSize;
    const int rank = MPI.CommWorldRank;
    MPI.Printf( stderr, "rank %d/%d: online\n", rank, size);
    
    //! prepare the fields setup
    fields_setup<layout1D> F;
    Y_SWAMP_DECL_VAR( F, "A", array1D<float>  );
    Y_SWAMP_DECL_VAR( F, "B", array1D<double> );
    Y_SWAMP_DECL_AUX( F, "C", array1D<double> );
    
    const layout1D full_layout(1,100);
    const layout1D L( full_layout.split(rank,size) );
    
    //! prepare ghosts setup
    ghosts_setup<coord1D> G;
    if( size > 1 )
    {
        G.lower.count = 2;
        G.lower.peer  = MPI.CommWorldPrev();
        G.upper.count = 2;
        G.upper.peer  = MPI.CommWorldNext();
        MPI.Printf(stderr, "rank %d> %d -> %d -> %d\n", rank, int(G.lower.peer), rank, int(G.upper.peer) );
    }
    else
    {
        G.local.count = 2;
    }
    
    //! create data
    dataspace<layout1D> D( L, G, F );
    array1D<float>  & A = D["A"].as< array1D<float> >();
    array1D<double> & B = D["B"].as< array1D<double> >();
    array1D<double> & C = D["C"].as< array1D<double> >();
    A.set_all( A, rank   );
    B.set_all( B, rank+1 );
    C.set_all( C, 0 );
    
    MPI.Printf(stderr," rank %d> localGhosts=%u, asyncGhosts=%u, #handles= %u\n", rank, unsigned( D.local_ghosts_count()), unsigned(D.async_ghosts_count()), unsigned(D.handles().size()) );
    MPI.Printf(stderr, "rank %d> #Requests= %u\n", rank, unsigned( D.num_requests()) );
    {
        const string filename = vformat("ini%d.%d.dat",rank,size);
        ios::ocstream fp( filename, false );
        for( unit_t i= A.lower; i <= A.upper; ++i )
        {
            fp("%d %g\n", i, A[i] );
        }
    }
    
    //! allocate communication data
    D.prepare_ghosts();
    mpi::Requests requests( D.num_requests() );
    
    
    //! start exchange
    _mpi::init_exchange(MPI,D,requests);
    MPI.Printf(stderr,"rank %d> launched requests\n", rank);
    //! do work on dataspace sync
    
    //! end exchange
    _mpi::wait_exchange(MPI,D,requests);
    MPI.Printf(stderr,"rank %d> received requests\n", rank);
    {
        const string filename = vformat("end%d.%d.dat",rank,size);
        ios::ocstream fp( filename, false );
        for( unit_t i= A.lower; i <= A.upper; ++i )
        {
            fp("%d %g\n", i, A[i] );
        }
    }
    MPI.Barrier(MPI_COMM_WORLD);
    
}
YOCTO_UNIT_TEST_DONE()
