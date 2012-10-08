#include "yocto/visit/interface.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
    
    const char * VisIt:: Simulation:: GenericCommandReg[] = { "halt", "run", "step" };
    const size_t VisIt:: Simulation:: GenericCommandNum   = 
    sizeof(VisIt:: Simulation::GenericCommandReg)/sizeof(VisIt:: Simulation::GenericCommandReg[0]);
    
    VisIt:: Simulation:: ~Simulation() throw()
    {
    }
    
    VisIt:: Simulation:: Simulation( const mpi &ref) :
    cycle(0),
    runMode( VISIT_SIMMODE_STOPPED ),
    runTime(0),
    done(false),
    iobuff( VisIt::IOBufferSize ),
    MPI( ref ),
    stepTime(-1),
    loopTime(-1),
    console(true),
    par_rank(  MPI.CommWorldRank ),
    par_size(  MPI.CommWorldSize ),
    par_last(  MPI.CommWorldLast ),
    parallel(  MPI.IsParallel    ),
    is_first(  MPI.IsFirst       ),
    is_final(  MPI.IsFinal       )
    {
    }
    
    void VisIt:: Simulation:: step()
    {
        const char *run_mode = VisItIsConnected() ? "[VisIt ONLINE ]" : "[Visit OFFLINE]";
        MPI.Printf0(stderr, "%s cycle= %6d\n", run_mode, cycle);
    }
    
    
    void VisIt::Simulation:: post_step() const
    {
        MPI.Printf0(stderr, "\tsteps/s= %10.3f | loops/s= %10.3f\n", 1.0 / stepTime, 1.0 / loopTime );
    }
    
    
#if 0
    void VisIt:: Simulation:: invite() const
    {
        if( console && runMode == VISIT_SIMMODE_STOPPED )
        {
            MPI.Printf0( stderr, "command> ");
        }
    }
#endif
    
    void VisIt:: Simulation:: performAlways( const string &cmd )
    {
        if(  cmd == "run"  )
        {
            runMode = VISIT_SIMMODE_RUNNING;
            return;
        }
        
        if( cmd == "halt" )
        {
            runMode = VISIT_SIMMODE_STOPPED;
            return;
        }
        
        if( cmd == "step" )
        {
            runMode = VISIT_SIMMODE_STOPPED;
            VisIt::OneStep(*this);
            return;
        }
        
        if( cmd == "quit" )
        {
            done = true;
            return;
        }
        
        // virtual call
        perform(cmd);
    }
    
    void VisIt:: Simulation:: perform( const string &cmd )
    {
        MPI.Printf(stderr,"rank %d> '%s'\n", par_rank, cmd.c_str());
    }
    
    void VisIt:: Simulation::  get_meta_data( visit_handle &md ) const
    {
        assert( VISIT_INVALID_HANDLE != md );
    }
    
    void VisIt:: Simulation:: add_generic_command( const string &name, visit_handle &md ) const
    {
        visit_handle cmd = VISIT_INVALID_HANDLE;
        if(VisIt_CommandMetaData_alloc(&cmd) == VISIT_OKAY)
        {
            VisIt_CommandMetaData_setName(cmd, name.c_str());
            VisIt_SimulationMetaData_addGenericCommand(md, cmd);
        }
        else
            throw exception("VisIt.Simulation.add_generic_command('%s') FAILURE", name.c_str());
    }
    
    void VisIt:: Simulation:: add_generic_command( const char *text, visit_handle &md ) const
    {
        const string name(text);
        add_generic_command(name, md);
    }
    
   
    
    visit_handle VisIt:: Simulation::  get_mesh( int domain, const string &name ) const
    {
        MPI.Printf0(stderr, "get_mesh(%d,'%s')\n", domain, name.c_str() );
        return VISIT_INVALID_HANDLE;
    }
    
    visit_handle VisIt:: Simulation::  get_variable( int domain, const string &name ) const
    { 
        MPI.Printf0(stderr, "get_variable(%d,'%s')\n", domain, name.c_str() );
        return VISIT_INVALID_HANDLE;
    }
    
    visit_handle VisIt:: Simulation:: get_curve( const string &name ) const
    {
        MPI.Printf0(stderr, "get_curve('%s')\n", name.c_str() );
        return VISIT_INVALID_HANDLE;
    }
    
    
}
