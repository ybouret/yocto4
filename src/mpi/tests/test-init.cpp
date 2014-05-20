#include "yocto/utest/run.hpp"

#include "yocto/mpi/mpi.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(init)
{
	
    YOCTO_MPI(SINGLE);
    MPI.CloseStdIO();
    MPI.Printf(stderr, "'%s' is ready\n",MPI.ProcessorName);
	MPI.Printf0(stderr, "ThreadLevel= %s\n", MPI.ThreadLevelName());
    
}

YOCTO_UNIT_TEST_DONE()
