#include "yocto/visit/visit.hpp"
#include "yocto/string/tokenizer.hpp"

namespace yocto
{
    
    
    static inline bool is_space(char C) throw() { return C ==  ' ' || C == '\t'; }
    static inline bool is_sep(char C) throw() { return C == ';'; }
    
    // format is 'cmd ARG1;...;ARGN'
    static inline
    string __parse_code(const string &code, vector<string> &args )
    {
        string cmd;
        tokenizer tkn(code);
        if( tkn.get_next(is_space) )
        {
            cmd = tkn.to_string();
            while(tkn.get_next(is_sep) )
            {
                const string a = tkn.to_string();
                args.push_back(a);
            }
        }
        
        return cmd;
    }
    
    void VisIt:: Execute(Simulation &sim, const string &code)
    {
        const mpi &MPI = sim.MPI;
        MPI.Printf0(stderr, "[VisIt]: executing <%s>\n", code.c_str() );
        vector<string> args(16,as_capacity);
        
        const string cmd = __parse_code(code,args);
        
        sim.execute_all(cmd,args);
        
        
        if( VisItIsConnected() )
        {
            VisItTimeStepChanged(); // force metadata reload
            VisItUpdatePlots();     // force plot update
        }
    }
    
    
}