#include "yocto/visit/interface.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
    
    const char * VisIt:: Simulation:: GenericCommandReg[] = { "halt", "run", "step" };
    const size_t VisIt:: Simulation:: GenericCommandNum = sizeof(VisIt:: Simulation::GenericCommandReg)/sizeof(VisIt:: Simulation::GenericCommandReg[0]);
    
    VisIt:: Simulation:: ~Simulation() throw()
    {
    }
    
    VisIt:: Simulation:: Simulation( mpi &MPI) :
    cycle(0),
    runMode( VISIT_SIMMODE_STOPPED ),
    done(false),
    connected(false),
    iobuff( VisIt::IOBufferSize ),
    console(true),
    par_rank( MPI.CommWorldRank ),
    par_size( MPI.CommWorldSize ),
    parallel(par_size>0),
    master(par_rank==0)
    {
    }
    
    void VisIt:: Simulation:: step()
    {
        mpi & MPI = *mpi::location();
        const char *run_mode = connected ? "[VisIt ONLINE ]" : "[Visit OFFLINE]";
        MPI.Printf0(stderr, "%s cycle= %6d\n", run_mode, cycle);
    }
    
    
    bool VisIt:: Simulation:: performAlways( const string &cmd )
    {
        if(  cmd == "run"  )
        {
            runMode = VISIT_SIMMODE_RUNNING;
            return true;
        }
        
        if( cmd == "halt" )
        {
            runMode = VISIT_SIMMODE_STOPPED;
            return true;
        }
        
        if( cmd == "step" )
        {
            runMode = VISIT_SIMMODE_STOPPED;
            VisIt::OneStep(*this);
            return true;
        }
        
        if( cmd == "quit" )
        {
            done = true;
            return true;
        }
        return false;
    }
    
    void VisIt:: Simulation:: perform( const string &cmd )
    {
        mpi & MPI = *mpi::location();
        MPI.Printf(stderr,"rank %d> '%s'\n", MPI.CommWorldRank, cmd.c_str());
    }
    
}
