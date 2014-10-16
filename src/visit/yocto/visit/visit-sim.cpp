#include "yocto/visit/visit.hpp"


namespace yocto
{
    
    VisIt:: Simulation:: ~Simulation() throw()
    {
    }
    
    
    VisIt:: Simulation:: Simulation(const mpi &ref, bool useConsole ) :
    cycle(0),
    runMode( VISIT_SIMMODE_STOPPED ),
    runTime(0),
    done(false),
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
    
    
}