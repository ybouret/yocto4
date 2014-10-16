#include "yocto/visit/visit.hpp"


namespace yocto
{
    
    // format is 'cmd ARG1;...;ARGN'
    static inline
    string __parse_code(const string &code, vector<string> &args )
    {
        return code;
    }
    
    void VisIt:: Execute(Simulation &sim, const string &code)
    {
        const mpi &MPI = sim.MPI;
        MPI.Printf(stderr, "[VisIt]: executing <%s>\n", code.c_str() );
        vector<string> args(16,as_capacity);
        
        const string cmd = __parse_code(code,args);
        
        sim.execute_all(cmd,args);
        
        
        if( VisItIsConnected() )
        {
            //VisItTimeStepChanged(); // force metadata reload
            //VisItUpdatePlots();     // force plot update
        }
    }
}