#include "yocto/utest/run.hpp"

#include "yocto/mpi/mpi.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(bcast)
{
	
	mpi &MPI = mpi::init( &argc, & argv );
	const int rank = MPI.CommWorldRank;
	const int size = MPI.CommWorldSize;
	MPI.Printf( stderr, "Bcast: Rank %d/%d is ready\n", rank, size ); 
	
	int msg = 0;
	if( 0 == rank )
	{
		msg = 7;
	}
	MPI.Printf0( stderr, "Broadcasting %d\n", msg );
	MPI.Bcast(&msg, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI.Barrier(MPI_COMM_WORLD);
	MPI.Printf( stderr, "Rank %d: msg=%d\n", rank, msg );
    MPI.Barrier(MPI_COMM_WORLD);
    
    msg = 10101;
    MPI.Printf0( stderr, "Broadcasting %d\n", msg );
    MPI.BcastAs<int>(msg, 0, MPI_COMM_WORLD);
    MPI.Barrier(MPI_COMM_WORLD);
	MPI.Printf( stderr, "Rank %d: msg=%d\n", rank, msg );
    MPI.Barrier(MPI_COMM_WORLD);
    
}
YOCTO_UNIT_TEST_DONE()


YOCTO_UNIT_TEST_IMPL(scatter)
{
	
	mpi &MPI = mpi::init( &argc, & argv );
	const int rank = MPI.CommWorldRank;
	const int size = MPI.CommWorldSize;
	MPI.Printf( stderr, "Scatter: Rank %d/%d is ready\n", rank, size ); 

	const size_t  N = 100;
	vector<float> src;
	if( rank == 0 ) 
	{
		src.make( N * size, 1.0f );
		for( size_t i=1; i <= src.size(); ++i ) src[i] = float(i);
	}
	vector<float> dst(N,0.0);
	
	float *sendbuff = rank == 0 ? &src[1] : NULL;
	MPI.Scatter(sendbuff, N, MPI_FLOAT, &dst[1], N, MPI_FLOAT, 0, MPI_COMM_WORLD);
	MPI.Printf( stderr, "Rank %d: dst[1]= %g\n", rank, dst[1] );
	
	MPI.Barrier(MPI_COMM_WORLD);
	
	
}
YOCTO_UNIT_TEST_DONE()

