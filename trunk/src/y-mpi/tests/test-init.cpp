#include "yocto/utest/run.hpp"

#include "yocto/mpi/mpi.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(init)
{
	
	
    mpi &MPI = mpi::init( &argc, & argv );
	
    MPI.Printf(stderr, "%u.%u '%s' is ready\n", unsigned(MPI.CommWorldSize), unsigned(MPI.CommWorldRank),MPI.ProcessorName );
	
	MPI.Finalize();
	
	
}

YOCTO_UNIT_TEST_DONE()
