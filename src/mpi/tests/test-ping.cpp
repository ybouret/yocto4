#define _CRT_SECURE_NO_WARNINGS 1
#include "yocto/utest/run.hpp"
#include "yocto/mpi/mpi.hpp"

using namespace yocto;
#include <cstring>

#define BLEN 16

YOCTO_UNIT_TEST_IMPL(ping)
{
	
	char obuff[BLEN];
	char ibuff[BLEN];
	
	memset( obuff, 0, sizeof(obuff) );
	memset( ibuff, 0, sizeof(ibuff) );
	strncpy( obuff, "Hello", sizeof(obuff) );
	
	
	mpi & MPI = mpi::init( &argc, &argv , MPI_THREAD_SINGLE);
	const int atSelf  = MPI.CommWorldRank;
	const int atRight = MPI.CommWorldNext();
	const int atLeft  = MPI.CommWorldPrev();
	MPI.Barrier( MPI_COMM_WORLD );
    MPI.Printf(stderr, "[MPI #%2d] Left=#%2d Right=%2d\n", atSelf, atLeft, atRight);
	if( MPI.CommWorldSize > 1 )
	{
		//-- send left->right
		MPI.Send( obuff, BLEN, MPI_BYTE, atRight, 0, MPI_COMM_WORLD );
		
		MPI_Status rs;
		MPI.Recv( ibuff, BLEN, MPI_BYTE,  atLeft, 0, MPI_COMM_WORLD, rs); 
		
	}
	
	MPI.Barrier( MPI_COMM_WORLD );
	
    MPI.Printf(stderr, "[MPI #%2d]: done\n", atSelf);
    MPI.Barrier(MPI_COMM_WORLD);
    MPI.WaitFor(0.2);
	MPI.Finalize();
}
YOCTO_UNIT_TEST_DONE()
