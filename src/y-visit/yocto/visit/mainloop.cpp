#include "yocto/visit/interface.hpp"
#include "yocto/exception.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/auto-ptr.hpp"

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
             * above can the rank 0 process send a VISIT_COMMAND_PROCESS * instruction to the non-rank 0 processes. */
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
        const mpi &  MPI = sim.MPI;
        char        *cmd = sim.iobuff();
        const size_t len = sim.iobuff.length();
        memset(cmd,len,0);
        if( sim.master )
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
            size_t num_in = strlen(cmd);
            while( num_in > 0 )
            {
                char &c = cmd[num_in-1];
                if( c == '\n' || c == '\r' )
                {
                    c = 0;
                    --num_in;
                }
                else 
                    break;
            }
        }
        
        //----------------------------------------------------------------------
        // broadcast the line
        //----------------------------------------------------------------------
        MPI.Bcast(cmd, len, MPI_CHAR, 0, MPI_COMM_WORLD);
        
        //----------------------------------------------------------------------
        // forward to the simulation
        //----------------------------------------------------------------------
        const string __cmd(cmd);
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
        //const mpi         &MPI = sim.MPI;
        
        //MPI.Printf0( stderr, "SimGetMetaData\n" );
        
        if( VisIt_SimulationMetaData_alloc(&md) == VISIT_OKAY) 
        {
            assert( VISIT_INVALID_HANDLE != md );
            
            /* Meta Data for Simulation */
            //MPI.Printf0( stderr, "\tsimulation info\n");
            VisIt_SimulationMetaData_setMode(md,sim.runMode);
            VisIt_SimulationMetaData_setCycleTime(md, sim.cycle,0);
            
            /* Specific Meta Data for the simulation */
            //MPI.Printf0( stderr, "\tuser's meta data\n");
            sim.get_meta_data(md);
            
            /* Create Generic Interface/Commands */
            //MPI.Printf0( stderr,"\tcommands: ");
            for(size_t i = 0; i <  VisIt::Simulation::GenericCommandNum; ++i)
            {
                visit_handle cmd = VISIT_INVALID_HANDLE;
                if(VisIt_CommandMetaData_alloc(&cmd) == VISIT_OKAY)
                {
                    const char *cmd_name = VisIt::Simulation::GenericCommandReg[i];
                    //MPI.Printf0(stderr,"'%s', ", cmd_name);
                    VisIt_CommandMetaData_setName(cmd, cmd_name);
                    VisIt_SimulationMetaData_addGenericCommand(md, cmd);
                }
            }
            //MPI.Printf0(stderr,"NULL\n");
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
            visit_handle hdl;
            int *iptr = NULL;
            
            iptr = (int *)malloc(sizeof(int));
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
    // Control Command Callback
    //
    //==========================================================================
    static inline 
    void ControlCommandCallback(const char *cmd, 
                                const char *args,
                                void       *cbdata)
    {
        assert(cbdata!=NULL);
        VisIt::Simulation &sim  = *(VisIt::Simulation *)cbdata;
        const string       todo = cmd;
        
        static mpi &MPI = *mpi::location();
        MPI.Printf0(stderr,"[VisIt::Command] '%s' (%s) \n", cmd, args ? args : "NO ARGS" );
        VisIt::Perform( sim, cmd );
    }
    
    
    //==========================================================================
    //
    //  make one step
    //
    //==========================================================================
    void VisIt:: OneStep( Simulation &sim )
    {
        ++sim.cycle;
        sim.step();
        if( VisItIsConnected() )
        {
            VisItTimeStepChanged();
            VisItUpdatePlots();
        }
    }
    
    
    //==========================================================================
    //
    //  process the command and forward to VisIt
    //
    //==========================================================================
    void VisIt:: Perform( Simulation &sim, const string &cmd )
    {
        sim.performAlways(cmd);
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
            if(sim.master)
            {
                visitstate = VisItDetectInput(blocking, fd );
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
