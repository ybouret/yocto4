#include "yocto/utest/run.hpp"
#include "yocto/visit/interface.hpp"

using namespace yocto;


static void
ui_run_clicked(void *cbdata)
{
    VisIt::Simulation *sim = (VisIt::Simulation *)cbdata;
    if(sim->master) fprintf(stderr,"ui_run_clicked\n");
    sim->runMode = VISIT_SIMMODE_RUNNING;
    VisItTimeStepChanged();
}

static void
ui_halt_clicked(void *cbdata)
{
    VisIt::Simulation *sim = (VisIt::Simulation *)cbdata;
    if(sim->master) fprintf(stderr,"ui_halt_clicked\n");
    sim->runMode = VISIT_SIMMODE_STOPPED;
    VisItTimeStepChanged();
}

static void
ui_step_clicked(void *cbdata)
{
    VisIt::Simulation *sim = (VisIt::Simulation *)cbdata;
    if(sim->master) fprintf(stderr,"ui_step_clicked\n");
    sim->runMode = VISIT_SIMMODE_STOPPED;
    VisIt::OneStep( *sim );
    VisItTimeStepChanged();
}

static void
ui_levels_changed(int value, void *cbdata)
{
    VisIt::Simulation *sim = (VisIt::Simulation *)cbdata;
    if(sim->master) fprintf(stderr,"ui_levels_changed: %d\n", value);
}

static void
ui_option_changed(int value, void *cbdata)
{
    VisIt::Simulation *sim = (VisIt::Simulation *)cbdata;
    if(sim->master) fprintf(stderr,"ui_option_changed: %d\n", value);
}

YOCTO_UNIT_TEST_IMPL(loop)
{
 
    const string       trace_name = "trace.dat";
    VisIt:: TraceFile  trace_file( trace_name );
    VisIt:: SetupEnvironment();
    
    const string sim_name    = "Simulation";
    const string sim_comment = "Simulation Comment";
    const string sim_path    = ".";
    const string sim_ui      = "loop.ui";
    mpi &MPI = mpi::init( &argc, &argv );
    VisIt:: SetupParallel( MPI, sim_name, sim_comment, sim_path, &sim_ui);
    VisIt::Simulation sim(MPI);
    
    VisItUI_clicked("RUN",         ui_run_clicked,     &sim);
    VisItUI_clicked("HALT",        ui_halt_clicked,    &sim);
    VisItUI_clicked("STEP",        ui_step_clicked,    &sim);

    VisItUI_valueChanged("LEVELS", ui_levels_changed, &sim);
    VisItUI_stateChanged("OPTION", ui_option_changed, &sim);
    
    VisIt:: MainLoop( sim );
    
    
}
YOCTO_UNIT_TEST_DONE()
