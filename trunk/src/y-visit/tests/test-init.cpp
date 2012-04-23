#include "yocto/utest/run.hpp"
#include "yocto/visit/interface.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(init)
{
 
    VisIt:: OpenTraceFile( "trace.txt" );
    VisIt:: SetupEnvironment();
    
    const string sim_name    = "Simulation";
    const string sim_comment = "Simulation Comment";
    const string sim_path    = ".";
    mpi &MPI = mpi::init( &argc, &argv );
    VisIt:: SetupParallel( MPI, sim_name, sim_comment, sim_path);
    
    VisIt::Simulation sim;
    
    VisIt:: MainLoop( MPI, sim, true);
    
    
}
YOCTO_UNIT_TEST_DONE()
