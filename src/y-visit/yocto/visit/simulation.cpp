#include "yocto/visit/interface.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
    
    const char * VisIt:: Simulation:: GenericCommandReg[] = { "halt", "run", "step" };
    const size_t VisIt:: Simulation:: GenericCommandNum = sizeof(VisIt:: Simulation::GenericCommandReg)/sizeof(VisIt:: Simulation::GenericCommandReg[0]);
    
    VisIt:: Simulation:: ~Simulation() throw()
    {
    }
    
    VisIt:: Simulation:: Simulation( const mpi &ref) :
    cycle(0),
    runMode( VISIT_SIMMODE_STOPPED ),
    done(false),
    connected(false),
    iobuff( VisIt::IOBufferSize ),
    MPI( ref ),
    console(true),
    par_rank( MPI.CommWorldRank ),
    par_size( MPI.CommWorldSize ),
    parallel(par_size>0),
    master(par_rank==0)
    {
    }
    
    void VisIt:: Simulation:: step()
    {
        const char *run_mode = connected ? "[VisIt ONLINE ]" : "[Visit OFFLINE]";
        MPI.Printf0(stderr, "%s cycle= %6d\n", run_mode, cycle);
    }
    
    void VisIt:: Simulation:: invite() const
    {
        if( console && runMode == VISIT_SIMMODE_STOPPED )
        {
            MPI.Printf0( stderr, "command> ");
        }
    }
    
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
    
}
