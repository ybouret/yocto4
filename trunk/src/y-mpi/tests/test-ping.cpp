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
	
	
	mpi & MPI = mpi::init( &argc, &argv );
	//std::cerr << "[MPI]: rank=" << MPI.CommWorldRank << " / " << MPI.CommWorldSize << " @" << MPI.ProcessorName << std::endl;
	
	const int atSelf  = MPI.CommWorldRank;
	const int atRight = MPI.CommWorldNext();
	const int atLeft  = MPI.CommWorldPrev();
	MPI.Barrier( MPI_COMM_WORLD );
	std::cerr << "[MPI #" << atSelf << "] Left=#" << atLeft << " Right=#" << atRight << std::endl;
	if( MPI.CommWorldSize > 1 )
	{
		//-- send left->right
		MPI.Send( obuff, BLEN, MPI_BYTE, atRight, 0, MPI_COMM_WORLD );
		
		MPI_Status rs;
		MPI.Recv( ibuff, BLEN, MPI_BYTE,  atLeft, 0, MPI_COMM_WORLD, rs); 
		
	}
	
	MPI.Barrier( MPI_COMM_WORLD );
	
	std::cerr << "[MPI]: rank=" << MPI.CommWorldRank << " done" << std::endl;
	MPI.Finalize();
}
YOCTO_UNIT_TEST_DONE()
