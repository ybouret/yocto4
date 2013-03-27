#include "yocto/utest/run.hpp"

#include "yocto/mpi/mpi.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(init)
{
	
	
    mpi &MPI = mpi::init( &argc, & argv );
	
    MPI.Printf(stderr, "'%s' is ready\n",MPI.ProcessorName);
	
	MPI.Finalize();
	
	
}

YOCTO_UNIT_TEST_DONE()
