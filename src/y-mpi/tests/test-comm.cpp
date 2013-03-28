#include "yocto/utest/run.hpp"

#include "yocto/mpi/comm-thread.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(comm)
{
	
	
    YOCTO_MPI;
    //MPI.CloseStdIO();

    mpi_comm_thread comm(MPI);
    
    comm.start(1);
    comm.start(0);
    
    MPI.Barrier(MPI_COMM_WORLD);

}
YOCTO_UNIT_TEST_DONE()
