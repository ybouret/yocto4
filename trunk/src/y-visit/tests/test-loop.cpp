#include "yocto/utest/run.hpp"
#include "yocto/visit/interface.hpp"

using namespace yocto;


void
ui_run_clicked(void *cbdata)
{
    VisIt::Simulation *sim = (VisIt::Simulation *)cbdata;
    if(sim->master) fprintf(stderr,"ui_run_clicked\n");
    sim->runMode = VISIT_SIMMODE_RUNNING;
    VisItTimeStepChanged();
}

YOCTO_UNIT_TEST_IMPL(loop)
{
 
    VisIt:: OpenTraceFile( "trace.dat" );
    VisIt:: SetupEnvironment();
    
    const string sim_name    = "Simulation";
    const string sim_comment = "Simulation Comment";
    const string sim_path    = ".";
    const string sim_ui      = "loop.ui";
    mpi &MPI = mpi::init( &argc, &argv );
    VisIt:: SetupParallel( MPI, sim_name, sim_comment, sim_path, &sim_ui);
    VisIt::Simulation sim(MPI);
    
    VisItUI_clicked("RUN", ui_run_clicked, &sim);
    VisIt:: MainLoop( MPI, sim, true);
    
    
}
YOCTO_UNIT_TEST_DONE()
