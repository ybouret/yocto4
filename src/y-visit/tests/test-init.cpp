#include "yocto/utest/run.hpp"
#include "yocto/visit/interface.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(init)
{
 
    const string sim_name    = "simulation";
    const string sim_comment = "Simulation Comment";
    const string sim_path    = ".";
    
    visit &VisIt = visit::SetupEnvironment(&argc, &argv, sim_name, sim_comment, sim_path);
    mpi   &MPI   = * mpi::location();
    MPI.Printf( stderr, "proc %d/%d is ready\n", MPI.CommWorldRank, MPI.CommWorldSize);
    if( VisIt.is_master )
    {
        VisItInitializeSocketAndDumpSimFile("simname",
                                            "Simulation Comment",
                                            "",
                                            NULL, NULL, NULL);
    }
    
    MPI.Barrier(MPI_COMM_WORLD);
    
}
YOCTO_UNIT_TEST_DONE()
