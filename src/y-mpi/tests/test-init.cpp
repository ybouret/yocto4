#include "yocto/utest/run.hpp"

#include "yocto/mpi/mpi.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(init)
{
	
	
    YOCTO_MPI;
    MPI.CloseStdIO();
    MPI.Printf(stderr, "'%s' is ready\n",MPI.ProcessorName);
	
	
	
}

YOCTO_UNIT_TEST_DONE()
