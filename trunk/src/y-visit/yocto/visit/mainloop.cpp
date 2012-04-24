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
    // Helper function for ProcessVisItCommand.
    //==========================================================================
    static void BroadcastSlaveCommand( int *command)
    {
        static mpi &MPI = *mpi::location();
        MPI.Bcast(command, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }
    
    //==========================================================================
    // Callback involved in command communication.
    //==========================================================================
    static void SlaveProcessCallback()
    {
        int command = VISIT_COMMAND_PROCESS;
        BroadcastSlaveCommand(&command);
    }
    
    
    static int ProcessVisItCommand( mpi &MPI, VisIt:: Simulation &sim )
    {
        int command = -1;
        if ( 0 == MPI.CommWorldRank )
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
    // Process Console.
    //==========================================================================
    static inline void ProcessConsole( mpi &MPI, VisIt:: Simulation &sim )
    {
        assert( sim.par_rank == MPI.CommWorldRank );
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
    // SimGetMetaData
    //==========================================================================
    static
    visit_handle SimGetMetaData(void *cbdata)
    {
        visit_handle       md  = VISIT_INVALID_HANDLE;
        VisIt::Simulation &sim = *(VisIt::Simulation *)cbdata;
        
        
        if(VisIt_SimulationMetaData_alloc(&md) == VISIT_OKAY) 
        {
            /* Meta Data for Simulation */
            VisIt_SimulationMetaData_setMode(md,sim.runMode);
            VisIt_SimulationMetaData_setCycleTime(md, sim.cycle,0);
            
            
            /* Create Generic Interface/Commands */
            for(size_t i = 0; i <  VisIt::Simulation::GenericCommandNum; ++i)
            {
                visit_handle cmd = VISIT_INVALID_HANDLE;
                if(VisIt_CommandMetaData_alloc(&cmd) == VISIT_OKAY)
                {
                    VisIt_CommandMetaData_setName(cmd, VisIt::Simulation::GenericCommandReg[i]);
                    VisIt_SimulationMetaData_addGenericCommand(md, cmd);
                }
            }
            
        }
        
        return md;
    }
    
    //==========================================================================
    // SimGetMetaData
    //==========================================================================
    static void
    ControlCommandCallback(const char *cmd, 
                           const char *args,
                           void       *cbdata)
    {
        VisIt::Simulation &sim = *(VisIt::Simulation *)cbdata;
        const string       todo = cmd;
        
        static mpi &MPI = *mpi::location();
        MPI.Printf0(stderr,"[VisIt::Command] '%s'\n", cmd );
        VisIt::Perform( sim, cmd );
    }
    
    
    void VisIt:: OneStep( Simulation &sim )
    {
        ++sim.cycle;
        sim.step();
    }
    
    void VisIt:: Perform( Simulation &sim, const string &cmd )
    {
        if( !sim.performAlways(cmd) )
            sim.perform(cmd);
        VisItTimeStepChanged();
    }
    
    void VisIt:: MainLoop( mpi &MPI, Simulation &sim, bool WithConsole )
    {
        
        MPI.Printf0(stderr, "[VisIt] Main Loop\n");
        if( WithConsole )
            MPI.Printf0(stderr, "[VisIt] Console is Active\n");
        
        int        visitstate = -1;
        const int  fd         = WithConsole ? fileno(stdin) : -1;
        
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
            // broadcast status
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
                    if(VisItAttemptToCompleteConnection())
                    {
                        MPI.Printf0(stderr, "[VisIt] Connected\n");
                        //------------------------------------------------------
                        // Setup callbacks for this connection
                        //------------------------------------------------------
                        VisItSetSlaveProcessCallback(SlaveProcessCallback);
                        VisItSetCommandCallback(ControlCommandCallback,(void*) &sim);
                        VisItSetGetMetaData(SimGetMetaData, (void*) &sim);
                        sim.connected = true;
                    }
                    else
                    {
                        MPI.Printf0(stderr, "[VisIt] Did not connect: %s\n", VisItGetLastError());
                        sim.connected = false; 
                    }
                    break;
                    
                case 2:
                    //----------------------------------------------------------
                    // VisIt wants to tell the engine something.
                    //----------------------------------------------------------
                    //sim.runMode = VISIT_SIMMODE_STOPPED;
                    if(!ProcessVisItCommand(MPI,sim))
                    {
                        /* Disconnect on an error or closed connection. */ 
                        MPI.Printf0(stderr,"[VisIt] Disconnected\n");
                        VisItDisconnect();
                        sim.connected = false;
                        //sim.runMode     = VISIT_SIMMODE_RUNNING;
                    }
                    break;
                    
                case 3: 
                    ProcessConsole(MPI, sim);
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
