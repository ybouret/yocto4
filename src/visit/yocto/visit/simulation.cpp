#include "yocto/visit/interface.hpp"
#include "yocto/exception.hpp"
#include "yocto/string/tokenizer.hpp"

#include <cstdlib>

#if 0
namespace yocto
{
    
    const char * VisIt:: Simulation:: GenericCommandReg[] = { "halt", "run", "step" };
    const size_t VisIt:: Simulation:: GenericCommandNum   =
    sizeof(VisIt:: Simulation::GenericCommandReg)/sizeof(VisIt:: Simulation::GenericCommandReg[0]);
    
    VisIt:: Simulation:: ~Simulation() throw()
    {
    }
    
    VisIt:: Simulation:: Simulation( const mpi &ref) :
    MPI(ref),
    cycle(0),
    runMode( VISIT_SIMMODE_STOPPED ),
    runTime(0),
    done(false),
    iobuff( VisIt::IOBufferSize ),
    stepTime(-1),
    loopTime(-1),
    commTime(0),
    console(true),
    par_rank(  MPI.CommWorldRank ),
    par_size(  MPI.CommWorldSize ),
    par_last(  MPI.CommWorldLast ),
    parallel(  MPI.IsParallel    ),
    is_first(  MPI.IsFirst       ),
    is_final(  MPI.IsFinal       ),
	cmdArgs()
    {
    }
    
    void VisIt:: Simulation:: no_cmd_args() throw()
    {
        cmdArgs.free();
    }
    
    void VisIt::Simulation:: add_cmd_arg( const string &a )
    {
        cmdArgs.push_back(a);
    }
    
    void VisIt::Simulation:: add_cmd_arg( const char *b )
    {
        const string a(b);
        add_cmd_arg(a);
    }
    
    
    void VisIt:: Simulation:: step_prolog()
    {
        const char *run_mode = VisItIsConnected() ? "[VisIt ONLINE ]" : "[Visit OFFLINE]";
        MPI.Printf0(stderr, "%s cycle= %6d\n", run_mode, cycle);
    }
    
    void VisIt:: Simulation:: step()
    {
        // do nothing
    }
    
    
    void VisIt::Simulation:: step_epilog()
    {
        MPI.Printf0(stderr,
                    "\tsteps/s= %8.2f | loops/s= %8.2f | CommTime = %6u usecs\n",
                    1.0 / stepTime,
                    1.0 / loopTime ,
                    commTime);
    }
    
    
    static inline
    bool is_cmd_sep( char c ) throw()
    {
        if( c == ';' )
            return true;
        return false;
    }
    
    string VisIt::Simulation:: parse_user_command( const string &user_cmd)
    {
        cmdArgs.free();
        tokenizer tkn(user_cmd);
        if( tkn.get_next( character<char>::is_space ) )
        {
            const string cmd( tkn.token(), tkn.units() );
            while( tkn.get_next( is_cmd_sep ) )
            {
                const string arg( tkn.token(), tkn.units() );
                cmdArgs.push_back( arg );
            }
            return cmd;
        }
        else
        {
            const string cmd;
            return cmd;
        }
    }
    
    void VisIt:: Simulation:: performAlways( const string &user_cmd )
    {
        const string         cmd  = parse_user_command(user_cmd);
        const array<string> &args = cmdArgs;
        
        MPI.Printf0(stderr, "command: <%s>", cmd.c_str());
        for( size_t i=1; i <= args.size(); ++i )
        {
            MPI.Printf0(stderr, " '%s'", args[i].c_str());
        }
        MPI.Printf0(stderr, "\n");
        
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
            runMode  = VISIT_SIMMODE_STOPPED;
            size_t n = 1;
            if( args.size() > 0 )
            {
                const char  *arg = args[1].c_str();
                n   = atol(arg);
                if(n<=0) n=1;
            }
            while(n-->0)
            {
                VisIt::OneStep(*this);
            }
            return;
        }
        
        if( cmd == "quit" )
        {
            done = true;
            return;
        }
        
        // virtual call
        perform(cmd,cmdArgs);
    }
    
    void VisIt:: Simulation:: perform( const string &cmd, const array<string> &args )
    {
        //MPI.Printf(stderr,"rank %d> '%s', %u args\n", par_rank, cmd.c_str(), unsigned(args.size()));
    }
    
    void VisIt:: Simulation::  get_meta_data( visit_handle &md ) const
    {
        if( VISIT_INVALID_HANDLE == md )
            throw exception("VisIt.Simulatyion.get_meta_data(NULL)");
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
#endif

