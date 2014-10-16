#include "yocto/visit/visit.hpp"


namespace yocto
{
    void VisIt:: Execute(Simulation &sim, const string &code)
    {
        const mpi &MPI = sim.MPI;
        MPI.Printf(stderr, "[VisIt]: executing <%s>\n", code.c_str() );
        
        if( code == "quit" )
        {
            sim.done = true;
            return;
        }
        
        
        if( VisItIsConnected() )
        {
            //VisItTimeStepChanged(); // force metadata reload
            //VisItUpdatePlots();     // force plot update
        }
    }
}