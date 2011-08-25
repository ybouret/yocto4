#include "yocto/utest/run.hpp"

#include "yocto/mpi/mpi.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(init)
{
	
	
	std::cerr << "Start Program" << std::endl;
	std::cerr << "\t sizeof(mpi)=" << sizeof(mpi) << std::endl;
	
	mpi &MPI = mpi::init( &argc, & argv );
	
	std::cerr << "size=" << MPI.CommWorldSize   << std::endl;
	std::cerr << "rank=" << MPI.CommWorldRank   << std::endl;
	std::cerr << "proc=" << MPI.ProcessorName   << std::endl;
	
	MPI.Barrier(MPI_COMM_WORLD);
	std::cerr << MPI.CommWorldRank + 1 << " / " <<  MPI.CommWorldSize << " is ready.." << std::endl;
	
	MPI.Barrier(MPI_COMM_WORLD);
	MPI.Finalize();
	std::cerr << "End Program" << std::endl;
	
	
}

YOCTO_UNIT_TEST_DONE()
