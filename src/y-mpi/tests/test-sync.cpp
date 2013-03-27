#include "yocto/utest/run.hpp"

#include "yocto/mpi/mpi.hpp"

using namespace yocto;



YOCTO_UNIT_TEST_IMPL(sync)
{
	
	
	mpi &MPI = mpi::init( &argc, & argv );
	std::cerr.flush();
	MPI.Printf( stderr, "From rank %d\n", MPI.CommWorldRank );
	std::cerr.flush();
	
}
YOCTO_UNIT_TEST_DONE()

