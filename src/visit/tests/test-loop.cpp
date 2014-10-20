#include "yocto/utest/run.hpp"
#include "yocto/visit/interface.hpp"
#include "yocto/sys/wtime.hpp"

using namespace yocto;

#if 0
static void
ui_run_clicked(void *cbdata)
{
    VisIt::Simulation *sim = (VisIt::Simulation *)cbdata;
    if(sim->is_first) fprintf(stderr,"ui_run_clicked\n");
    sim->runMode = VISIT_SIMMODE_RUNNING;
    VisItTimeStepChanged();
}

static void
ui_halt_clicked(void *cbdata)
{
    VisIt::Simulation *sim = (VisIt::Simulation *)cbdata;
    if(sim->is_first) fprintf(stderr,"ui_halt_clicked\n");
    sim->runMode = VISIT_SIMMODE_STOPPED;
    VisItTimeStepChanged();
}

static void
ui_step_clicked(void *cbdata)
{
    VisIt::Simulation *sim = (VisIt::Simulation *)cbdata;
    if(sim->is_first) fprintf(stderr,"ui_step_clicked\n");
    sim->runMode = VISIT_SIMMODE_STOPPED;
    VisIt::OneStep( *sim );
    VisItTimeStepChanged();
}

static void
ui_levels_changed(int value, void *cbdata)
{
    VisIt::Simulation *sim = (VisIt::Simulation *)cbdata;
    if(sim->is_first) fprintf(stderr,"ui_levels_changed: %d\n", value);
}

static void
ui_option_changed(int value, void *cbdata)
{
    VisIt::Simulation *sim = (VisIt::Simulation *)cbdata;
    if(sim->is_first) fprintf(stderr,"ui_option_changed: %d\n", value);
}


static
void ui_do_nothing(int,void*)
{
    
}


class MySim : public VisIt:: Simulation
{
public:
    explicit MySim( const mpi &ref ) : VisIt:: Simulation( ref )
    {
        
    }
    
    virtual void get_meta_dat( visit_handle &md ) const
    {
        add_generic_command("quit",md);
    }
    
    virtual ~MySim() throw() 
    {
        
    }
    
    virtual void step()
    {
        static const mpi & MPI = mpi::instance();
        // simulate work
        VisIt:: Simulation:: step();
        wtime::sleep( 0.2 );
        
        if( VisItIsConnected() )
        {
            
            MPI.Printf0(stderr, "update UI\n" );
            if(is_first)
            {
                VisItUI_setValueI("LEVELS",      cycle % 100, 1);
                VisItUI_setValueI("progressBar", cycle % 100, 1);
            }
            VisItTimeStepChanged();
        }
    }
    
private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(MySim);
};

#endif

YOCTO_UNIT_TEST_IMPL(loop)
{

#if 0
    const string sim_name    = "Simulation";
    const string sim_comment = "Simulation Comment";
    const string sim_path    = ".";
    const string sim_ui      = "loop.ui";
    YOCTO_MPI(MPI_THREAD_SINGLE);
    
    const string       trace_name = "trace.dat";
    VisIt:: TraceFile  trace_file( MPI.CommWorldRank, trace_name );
    
    VisIt:: SetupParallel( MPI, sim_name, sim_comment, sim_path, &sim_ui);
    MySim sim(MPI);
    
    //-- it looks like all widgets must be declared...
    VisItUI_clicked("RUN",         ui_run_clicked,     &sim);
    VisItUI_clicked("HALT",        ui_halt_clicked,    &sim);
    VisItUI_clicked("STEP",        ui_step_clicked,    &sim);
    
    VisItUI_valueChanged("LEVELS", ui_levels_changed, &sim);
    VisItUI_stateChanged("OPTION", ui_option_changed, &sim);
    VisItUI_valueChanged("progressBar", ui_do_nothing, &sim);
    
    VisIt:: MainLoop( sim );
#endif
    
}
YOCTO_UNIT_TEST_DONE()
