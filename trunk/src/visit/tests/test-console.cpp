
#include "yocto/visit/visit.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;

static void
ui_run_clicked(void *cbdata)
{
    VisIt::Simulation *sim = (VisIt::Simulation *)cbdata;
    sim->MPI.Printf(stderr,"ui_run_clicked\n");
    sim->runMode = VISIT_SIMMODE_RUNNING;
    VisItTimeStepChanged();
}

static void
ui_halt_clicked(void *cbdata)
{
    VisIt::Simulation *sim = (VisIt::Simulation *)cbdata;
    sim->MPI.Printf(stderr,"ui_halt_clicked\n");
    sim->runMode = VISIT_SIMMODE_STOPPED;
    VisItTimeStepChanged();
}

static void
ui_step_clicked(void *cbdata)
{
    VisIt::Simulation *sim = (VisIt::Simulation *)cbdata;
    sim->MPI.Printf(stderr,"ui_step_clicked\n");
    sim->runMode = VISIT_SIMMODE_STOPPED;
    //VisIt::OneStep( *sim );
    VisItTimeStepChanged();
}

static void
ui_levels_changed(int value, void *cbdata)
{
    VisIt::Simulation *sim = (VisIt::Simulation *)cbdata;
    sim->MPI.Printf(stderr,"ui_levels_changed: %d\n", value);
}

static void
ui_option_changed(int value, void *cbdata)
{
    VisIt::Simulation *sim = (VisIt::Simulation *)cbdata;
    sim->MPI.Printf(stderr,"ui_option_changed: %d\n", value);
}


static
void ui_do_nothing(int,void*)
{
    
}


YOCTO_UNIT_TEST_IMPL(console)
{
    const string sim_name    = "TestConsoleAndUI";
    const string sim_comment = "Basic Do Nothing Simulation";
    const string sim_path    = ".";
    const string sim_ui      = "loop.ui";

    YOCTO_MPI(MPI_THREAD_SINGLE);
    
    VisIt::TraceFile trace(MPI,"trace.dat");
    VisIt::SetupParallel(MPI,sim_name,sim_comment,sim_path, &sim_ui);
    
    VisIt::Simulation sim(MPI);
    
    //-- it looks like all widgets must be declared...
    VisItUI_clicked("RUN",         ui_run_clicked,     &sim);
    VisItUI_clicked("HALT",        ui_halt_clicked,    &sim);
    VisItUI_clicked("STEP",        ui_step_clicked,    &sim);
    
    VisItUI_valueChanged("LEVELS", ui_levels_changed, &sim);
    VisItUI_stateChanged("OPTION", ui_option_changed, &sim);
    VisItUI_valueChanged("progressBar", ui_do_nothing, &sim);
    
    //-- go
    VisIt::Loop(sim);
    
}
YOCTO_UNIT_TEST_DONE()

