#include "yocto/visit/visit.hpp"


namespace yocto
{
    const char * VisIt::Simulation:: GenericCommandReg[] =
    {
        "step",
        "run",
        "halt"
    };
    
    const size_t VisIt:: Simulation:: GenericCommandNum =
    sizeof(VisIt::Simulation:: GenericCommandReg)/sizeof(VisIt::Simulation:: GenericCommandReg[0]);
    
    
    VisIt:: Simulation:: ~Simulation() throw()
    {
    }
    
    
    VisIt:: Simulation:: Simulation(const mpi &ref, bool useConsole ) :
    cycle(0),
    runMode( VISIT_SIMMODE_STOPPED ),
    runTime(0),
    done(false),
    iobuff( IOLength ),
    console(useConsole),
    MPI(ref),
    par_size( MPI.CommWorldSize ),
    par_rank( MPI.CommWorldRank ),
    par_last( MPI.CommWorldLast ),
    parallel( MPI.IsParallel    ),
    is_first( MPI.IsFirst ),
    is_final( MPI.IsFinal )
    {
    }
    
    
    void VisIt:: Simulation:: add_generic_command(const string &name, visit_handle &md) const
    {
        assert(VISIT_INVALID_HANDLE != md );
        visit_handle cmd = VISIT_INVALID_HANDLE;
        if(VisIt_CommandMetaData_alloc(&cmd) == VISIT_OKAY)
        {
            VisIt_CommandMetaData_setName(cmd, name.c_str());
            VisIt_SimulationMetaData_addGenericCommand(md, cmd);
        }
        else
            throw exception("add_generic_command('%s') FAILURE", name.c_str());
    }
    
    void VisIt:: Simulation:: add_generic_command(const char *name, visit_handle &md) const
    {
        const string NAME(name);
        add_generic_command(NAME,md);
    }
    
    
    void VisIt:: Simulation:: execute_all(const string cmd, const array<string> &args)
    {
        MPI.Printf(stderr, "[VisIt.Simulation.execute_all] '%s'\n", cmd.c_str());
        
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
        
        if( cmd == "quit" )
        {
            done = true;
            return;
        }
        
        if( cmd == "step" )
        {
         
            return;
        }
        
        execute(cmd,args);
        
    }
    
    void VisIt:: Simulation:: execute(const string cmd, const array<string> &args)
    {
        MPI.Printf0(stderr, "[VisIt.Simulation.execute] '%s'\n", cmd.c_str());
    }
    
    void VisIt::Simulation:: get_meta_data( visit_handle &md ) const
    {
        if(VISIT_INVALID_HANDLE==md)
            throw yocto::exception("Invalid VisIt Handle in sim.get_meta_data");
        
    }

    
    
}