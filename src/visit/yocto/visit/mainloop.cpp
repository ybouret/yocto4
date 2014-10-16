#include "yocto/visit/interface.hpp"
#include "yocto/exception.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ptr/auto.hpp"

#include <cstdlib>

#if 0
namespace yocto
{
    
#define VISIT_COMMAND_PROCESS 0
#define VISIT_COMMAND_SUCCESS 1
#define VISIT_COMMAND_FAILURE 2
    
    //==========================================================================
    //
    // Helper function for ProcessVisItCommand.
    //
    //==========================================================================
    static inline
    void BroadcastSlaveCommand( int *command)
    {
        static mpi &MPI = *mpi::location();
        assert(command!=NULL);
        MPI.Bcast(command, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }
    
    //==========================================================================
    //
    // Callback involved in command communication.
    //
    //==========================================================================
    static inline
    void SlaveProcessCallback()
    {
        int command = VISIT_COMMAND_PROCESS;
        BroadcastSlaveCommand(&command);
    }
    
    
    static inline
    int ProcessVisItCommand( VisIt:: Simulation &sim )
    {
        int command = -1;
        if ( 0 == sim.par_rank )
        {
            const int success = VisItProcessEngineCommand();
            if (success)
            {
                command = VISIT_COMMAND_SUCCESS;
                BroadcastSlaveCommand(&command);
                return 1;
            }
            else
            {
                command = VISIT_COMMAND_FAILURE;
                BroadcastSlaveCommand(&command);
                return 0;
            }
        }
        else
        {
            /* Note: only through the SlaveProcessCallback callback
             * above can the rank 0 process send a VISIT_COMMAND_PROCESS 
             * instruction to the non-rank 0 processes. */
            while(true)
            {
                BroadcastSlaveCommand(&command);
                switch (command)
                {
                    case VISIT_COMMAND_PROCESS:
                        (void)VisItProcessEngineCommand();
                        break;
                    case VISIT_COMMAND_SUCCESS:
                        return 1;
                    case VISIT_COMMAND_FAILURE:
                        return 0;
                }
            }
        }
        return 1;
    }
    
    
    //==========================================================================
    //
    // Process Console.
    //
    //==========================================================================
    static inline
    void ProcessConsole( VisIt:: Simulation &sim )
    {
        const mpi &  MPI = mpi::instance();
        char        *cmd = sim.iobuff();
        const size_t len = sim.iobuff.length();
        size_t       num = 0;
        memset(cmd,len,0);
        
        if( sim.is_first )
        {
            //------------------------------------------------------------------
            // capture the line
            //------------------------------------------------------------------
            if( NULL == fgets(cmd,len,stdin) )
            {
                //-- EOF
                snprintf( cmd, len, "quit" );
            }
            
            //------------------------------------------------------------------
            // clean the line
            //------------------------------------------------------------------
            num = strlen(cmd);
            while( num > 0 )
            {
                char &c = cmd[num-1];
                if( c == '\n' || c == '\r' )
                {
                    c = 0;
                    --num;
                }
                else
                    break;
            }
        }
        
        //----------------------------------------------------------------------
        // broadcast the line
        //----------------------------------------------------------------------
        MPI.Bcast<size_t>(num, 0, MPI_COMM_WORLD);
        if(num>0)
        {
            MPI.Bcast(cmd, num, MPI_CHAR, 0, MPI_COMM_WORLD);
        }
        
        //----------------------------------------------------------------------
        // forward to the simulation with a local string
        //----------------------------------------------------------------------
        const string __cmd(cmd,num);
        MPI.Printf0(stderr, "[VisIt::Console] '%s'\n", __cmd.c_str());
        VisIt::Perform(sim,__cmd);
    }
    
    
    //==========================================================================
    //
    // SimGetMetaData callback
    //
    //==========================================================================
    static
    visit_handle SimGetMetaData(void *cbdata)
    {
        assert(cbdata!=NULL);
        visit_handle       md  = VISIT_INVALID_HANDLE;
        VisIt::Simulation &sim = *(VisIt::Simulation *)cbdata;
        
        
        if( VisIt_SimulationMetaData_alloc(&md) == VISIT_OKAY)
        {
            assert( VISIT_INVALID_HANDLE != md );
            
            /* Meta Data for Simulation */
            VisIt_SimulationMetaData_setMode(md,sim.runMode);
            VisIt_SimulationMetaData_setCycleTime(md, sim.cycle,sim.runTime);
            
            
            /* Create Generic Interface/Commands */
            for(size_t i = 0; i <  VisIt::Simulation::GenericCommandNum; ++i)
            {
                visit_handle cmd = VISIT_INVALID_HANDLE;
                if(VisIt_CommandMetaData_alloc(&cmd) == VISIT_OKAY)
                {
                    const char *cmd_name = VisIt::Simulation::GenericCommandReg[i];
                    VisIt_CommandMetaData_setName(cmd, cmd_name);
                    VisIt_SimulationMetaData_addGenericCommand(md, cmd);
                }
            }
            
            /* Specific Meta Data for the simulation */
            sim.get_meta_data(md);
            
        }
        
        return md;
    }
    
    //==========================================================================
    //
    // SimGetDomainList callback
    //
    //==========================================================================
    static
    visit_handle SimGetDomainList(const char *name, void *cbdata)
    {
        assert(cbdata);
        VisIt::Simulation &sim = *(VisIt::Simulation *)cbdata;
        visit_handle h = VISIT_INVALID_HANDLE;
        if(VisIt_DomainList_alloc(&h) != VISIT_ERROR)
        {
            visit_handle hdl = 0;
            int *iptr = NULL;
            
            iptr  = (int *)malloc(sizeof(int));
            *iptr = sim.par_rank;
            
            if(VisIt_VariableData_alloc(&hdl) == VISIT_OKAY)
            {
                VisIt_VariableData_setDataI(hdl, VISIT_OWNER_VISIT, 1, 1, iptr);
                VisIt_DomainList_setDomains(h, sim.par_size, hdl);
            }
        }
        return h;
    }
    
    //==========================================================================
    //
    // SimGetVariable callback
    //
    //==========================================================================
    static
    visit_handle SimGetVariable(int domain, const char *name, void *cbdata)
    {
        assert(cbdata);
        VisIt::Simulation &sim = *(VisIt::Simulation *)cbdata;
        const string       id(name);
        return sim.get_variable(domain, id);
    }
    
    
    //==========================================================================
    //
    // Control Command Callback
    //
    //==========================================================================
    static inline
    void ControlCommandCallback(const char *cmd,
                                const char *args,
                                void       *cbdata)
    {
        assert(cbdata!=NULL);
        assert(cmd!=NULL);
        VisIt::Simulation &sim  = *(VisIt::Simulation *)cbdata;
        string             todo = cmd;
        if(args!=NULL)
        {
            todo += ' ';
            todo += args;
        }
        static const mpi &MPI = sim.MPI;
        MPI.Printf0(stderr,"[VisIt::Command] '%s'\n", todo.c_str());
        VisIt::Perform(sim,todo);
    }
    
    //==========================================================================
    //
    // SimGetCurve callback
    //
    //==========================================================================
    static inline
    visit_handle SimGetCurve( const char *name, void *cbdata )
    {
        VisIt::Simulation &sim = *(VisIt::Simulation *)cbdata;
        const string       id(name);
        return sim.get_curve(id);
    }
    
    
    //==========================================================================
    //
    //  make one step
    //
    //==========================================================================
    void VisIt:: OneStep( Simulation &sim )
    {
        static const mpi & MPI = sim.MPI;
        const double t0        = MPI_Wtime();
        ++sim.cycle;
        
        //-- display/precompute something
        sim.step_prolog();
        
        //-- effective step
        {
            const uint64_t mu0  = MPI.CommTime;
            const double   t1   = MPI_Wtime();
            sim.step();
            sim.stepTime  = MPI_Wtime() - t1;
            sim.commTime  = unsigned(MPI.CommTime - mu0);
        }
        if( VisItIsConnected() )
        {
            VisItTimeStepChanged();
            VisItUpdatePlots();
        }
        
        sim.loopTime  = MPI_Wtime() - t0;
        //-- display/postcompute something
        sim.step_epilog();
    }
    
    
    //==========================================================================
    //
    //  process the command and forward to VisIt
    //
    //==========================================================================
    void VisIt:: Perform( Simulation &sim, const string &user_cmd )
    {
        sim.performAlways(user_cmd);
        if( VisItIsConnected() )
        {
            VisItTimeStepChanged();
            VisItUpdatePlots();
        }
    }
    
    //==========================================================================
    //
    //  Getting the meshes
    //
    //==========================================================================
    static inline
    visit_handle SimGetMesh( int domain, const char *name, void *cbdata )
    {
        assert(cbdata!=NULL);
        VisIt::Simulation &sim = *(VisIt::Simulation *)cbdata;
        const string mesh_name(name);
        return sim.get_mesh( domain, mesh_name );
    }
    
    
    void VisIt:: MainLoop( Simulation &sim )
    {
        const mpi &MPI = sim.MPI;
        MPI.Printf0(stderr, "[VisIt] Main Loop\n");
        
        
        int        visitstate = -1;
        const int  fd         = sim.console ? fileno(stdin) : -1;
        if( sim.console )
        {
            MPI.Printf0(stderr, "[VisIt] Console is Active\n");
        }
        
        do
        {
            //------------------------------------------------------------------
            // Get input from VisIt or timeout so the simulation can run.
            //------------------------------------------------------------------
            const int blocking = ( sim.runMode == VISIT_SIMMODE_RUNNING) ? 0 : 1;
            if(sim.is_first)
            {
                visitstate = VisItDetectInput(blocking,fd);
            }
            
            //------------------------------------------------------------------
            // broadcast status from master rank=0
            //------------------------------------------------------------------
            MPI.Bcast(&visitstate, 1, MPI_INT, 0, MPI_COMM_WORLD);
            
            //------------------------------------------------------------------
            // act according to status
            //------------------------------------------------------------------
            switch( visitstate )
            {
                    
                case 0:
                    //----------------------------------------------------------
                    // There was no input from VisIt, return control to sim.
                    //----------------------------------------------------------
                    OneStep(sim);
                    break;
                    
                case 1:
                    MPI.Printf0(stderr,"[VisIt] Connecting...\n");
                    //----------------------------------------------------------
                    // VisIt is trying to connect to sim
                    //----------------------------------------------------------
                    if( VisItAttemptToCompleteConnection() )
                    {
                        MPI.Printf0(stderr, "[VisIt] Connected\n");
                        void *cbdata = (void*) &sim;
                        //------------------------------------------------------
                        // Setup callbacks for this connection
                        //------------------------------------------------------
                        VisItSetSlaveProcessCallback(SlaveProcessCallback);
                        VisItSetCommandCallback(ControlCommandCallback,cbdata);
                        VisItSetGetMetaData(SimGetMetaData,cbdata);
                        VisItSetGetMesh(SimGetMesh, cbdata);
                        VisItSetGetDomainList(SimGetDomainList,cbdata);
                        VisItSetGetVariable(SimGetVariable,cbdata);
                        VisItSetGetCurve(SimGetCurve,cbdata);
                        
                        //------------------------------------------------------
                        // stop simulation on connect
                        //------------------------------------------------------
                        sim.runMode = VISIT_SIMMODE_STOPPED;
                        VisItTimeStepChanged();
                    }
                    else
                    {
                        MPI.Printf0(stderr, "[VisIt] Did not connect: %s\n", VisItGetLastError());
                    }
                    break;
                    
                case 2:
                    //----------------------------------------------------------
                    // VisIt wants to tell the engine something.
                    //----------------------------------------------------------
                    if(!ProcessVisItCommand(sim))
                    {
                        /* Disconnect on an error or closed connection. */
                        MPI.Printf0(stderr,"[VisIt] Disconnected\n");
                        VisItDisconnect();
                    }
                    break;
                    
                case 3:
                    ProcessConsole(sim);
                    break;
                    
                case -1:
                case -2:
                case -3:
                case -4:
                case -5:
                    throw exception("VisItDetectInput Error(%d): %s",visitstate,VisItGetLastError());
                    
                default:
                    throw exception("VisItDetectInput Unexpected Error(%d): %s", visitstate, VisItGetLastError());
                    
            }
            
        }
        while ( !sim.done );
    }
}
#endif

