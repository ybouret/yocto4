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
    ellapsed(0),
    done(false),
    iobuff( IOLength ),
    console(useConsole),
    MPI(ref),
    par_size( MPI.CommWorldSize ),
    par_rank( MPI.CommWorldRank ),
    par_last( MPI.CommWorldLast ),
    parallel( MPI.IsParallel    ),
    is_first( MPI.IsFirst       ),
    is_final( MPI.IsFinal       )
    {
    }
    
    
    void VisIt:: Simulation:: add_generic_command(const string &name,
                                                  visit_handle &md) const
    {
        assert(VISIT_INVALID_HANDLE != md );
        visit_handle cmd = VISIT_INVALID_HANDLE;
        if(VisIt_CommandMetaData_alloc(&cmd) == VISIT_OKAY)
        {
            VisIt_CommandMetaData_setName(cmd, name.c_str());
            VisIt_SimulationMetaData_addGenericCommand(md, cmd);
        }
        else
        {
            throw exception("add_generic_command('%s') FAILURE", name.c_str());
        }
    }
    
    
    void VisIt:: Simulation:: add_generic_command(const char  *name,
                                                  visit_handle &md) const
    {
        const string NAME(name);
        add_generic_command(NAME,md);
    }
    
    
    static inline
    size_t get_tolerant_count( const string &arg ) throw()
    {
        const size_t len = arg.size();
        if(len>0)
        {
            size_t ans = 0;
            for(size_t i=0;i<len;++i)
            {
                const int C = arg[i];
                if(C<'0'||C>'9')
                    return 1;
                ans *= 10;
                ans += (C-'0');
            }
            return ans;
        }
        return 1;
    }
    
    void VisIt:: Simulation:: execute_all(const string         cmd,
                                          const array<string> &args)
    {
        string show = "[Visit] [Execute] ";
        show += "'" + cmd +  "'";
        for(size_t i=1;i<=args.size();++i)
        {
            show += '/' + args[i];
        }
        MPI.Printf0(stderr, "%s\n", show.c_str() );
        
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
            size_t n=1;
            if(args.size()>0)
            {
                n = get_tolerant_count(args[1]);
            }
            runMode = VISIT_SIMMODE_STOPPED;
            while(n-- > 0)
            {
                VisIt::OneStep(*this);
            }
            return;
        }
        
        execute(cmd,args);
        
    }
    
    
    void VisIt:: Simulation:: step()
    {
        const char *run_mode = VisItIsConnected() ? "[VisIt ONLINE ]" : "[Visit OFFLINE]";
        MPI.Printf0(stderr, "%s cycle= %6d\n", run_mode, cycle);
    }
    
    void VisIt:: Simulation:: execute(const string cmd, const array<string> &args)
    {
        MPI.Printf0(stderr, "[Simulation] '%s'\n", cmd.c_str());
    }
    
    
    void VisIt::Simulation:: get_meta_data( visit_handle &md ) const
    {
        if(VISIT_INVALID_HANDLE==md)
            throw yocto::exception("Invalid VisIt Handle in sim.get_meta_data");
        
    }
    
    
    visit_handle VisIt::Simulation:: get_mesh( int domain, const string &name) const
    {
        MPI.Printf(stderr,"\tget_mesh(%d,'%s')", domain, name.c_str() );
        return VISIT_INVALID_HANDLE;
    }
    
    
    visit_handle VisIt:: Simulation:: get_variable( int domain, const string &name ) const
    {
        MPI.Printf(stderr,"\tget_variable(%d,'%s')", domain, name.c_str() );
        return VISIT_INVALID_HANDLE;
    }
    
    
    visit_handle  VisIt:: Simulation::get_curve(const string &name) const
    {
        MPI.Printf(stderr,"\tget_curve('%s')", name.c_str() );
        return VISIT_INVALID_HANDLE;
    }
    
}

