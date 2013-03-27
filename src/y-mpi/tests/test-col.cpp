#include "yocto/utest/run.hpp"

#include "yocto/mpi/mpi.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(bcast)
{
	
	mpi &MPI = mpi::init( &argc, & argv );
	const int rank = MPI.CommWorldRank;
	//const int size = MPI.CommWorldSize;
	MPI.Printf( stderr, "Bcast is ready\n" );
	
	int msg = 0;
	if( 0 == rank )
	{
		msg = 7;
	}
	MPI.Printf0( stderr, "Broadcasting %d\n", msg );
    uint64_t mu0 = MPI.CommTime;
	MPI.Bcast(&msg, 1, MPI_INT, 0, MPI_COMM_WORLD);
    uint64_t mu  = MPI.CommTime - mu0;
	MPI.Printf( stderr, "msg=%d\n",msg );
    MPI.Barrier(MPI_COMM_WORLD);
    MPI.WaitFor(0.2);
    MPI.Printf0(stderr, "\nin %g usec\n\n", double(mu));
    MPI.Barrier(MPI_COMM_WORLD);
    
    msg = 0;
    if( 0 == rank )
        msg = 10101;
    MPI.Printf0( stderr, "Broadcasting %d\n", msg );
    mu0 = MPI.CommTime;
    MPI.Bcast<int>(msg, 0, MPI_COMM_WORLD);
    mu  = MPI.CommTime - mu0;
	MPI.Printf( stderr, "msg=%d\n",msg );
    MPI.Barrier(MPI_COMM_WORLD);
    MPI.WaitFor(0.2);
    MPI.Printf0(stderr, "\nin %g usec\n\n", double(mu));
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
		for( size_t i=1; i <= src.size(); ++i )
            src[i] = float(i);
	}
	vector<float> dst(N,0.0f);
	
	float *sendbuff = rank == 0 ? &src[1] : NULL;
    size_t sendsize = rank == 0 ? N       : 0;
	MPI.Scatter(sendbuff, sendsize, MPI_FLOAT, &dst[1], N, MPI_FLOAT, 0, MPI_COMM_WORLD);
	MPI.Printf( stderr, "Rank %d: dst[1]= %4g, dst[N]=%4g\n", rank, dst[1], dst[N] );
	
	MPI.Barrier(MPI_COMM_WORLD);
	
	
}
YOCTO_UNIT_TEST_DONE()

