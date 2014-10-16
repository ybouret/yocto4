#include "yocto/visit/visit.hpp"


namespace yocto
{
    
    
    namespace
    {
        
        //______________________________________________________________________
        //
        //
        // Processing commands
        //
        //______________________________________________________________________
        
#define VISIT_COMMAND_PROCESS 0
#define VISIT_COMMAND_SUCCESS 1
#define VISIT_COMMAND_FAILURE 2
        
        // broadcasting a command
        static inline void BroadcastSlaveCommand( int *command)
        {
            assert(command);
            const int err = MPI_Bcast(command, 1, MPI_INT, 0, MPI_COMM_WORLD);
            if( err != MPI_SUCCESS )
                throw mpi::exception(err,"BroadcastSlaveCommand '%d'", *command);
        }
        
        // broadcast VISIT_COMMAND_PROCESS
        static inline void SlaveProcessCallback()
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
        
        
        
        
        static inline void ControlCommandCallback(const char *cmd,
                                                  const char *args,
                                                  void       *cbdata)
        {
            assert(cbdata);
            assert(cmd);
            VisIt::Simulation &sim  = *(VisIt::Simulation *)cbdata;
            string             todo = cmd;
            if(args!=NULL)
            {
                todo += ' ';
                todo += args;
            }
            static const mpi &MPI = sim.MPI;
            MPI.Printf0(stderr,"[VisIt::Command] '%s'\n", todo.c_str());
            VisIt::Execute(sim,todo);
        }
        
        //______________________________________________________________________
        //
        //
        // Process Console.
        //
        //______________________________________________________________________
        static inline
        void ProcessConsole( VisIt:: Simulation &sim )
        {
            const mpi &  MPI = mpi::instance();
            char         cmd[1024];
            const size_t len = sizeof(cmd);
            size_t       num = 0;
            memset(cmd,len,0);
            
            if( sim.is_first )
            {
                //______________________________________________________________
                //
                // capture the line
                //______________________________________________________________
                if( NULL == fgets(cmd,len,stdin) )
                {
                    //-- EOF
                    snprintf( cmd, len, "quit" );
                }
                
                //______________________________________________________________
                //
                // clean the line
                //______________________________________________________________
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
            
            //__________________________________________________________________
            //
            // broadcast the line
            //__________________________________________________________________
            MPI.Bcast<size_t>(num, 0, MPI_COMM_WORLD);
            if(num>0)
            {
                MPI.Bcast(cmd, num, MPI_CHAR, 0, MPI_COMM_WORLD);
            }
            
            //__________________________________________________________________
            //
            // forward to the simulation with a local string
            //__________________________________________________________________
            const string code(cmd,num);
            MPI.Printf0(stderr, "[VisIt::Console] '%s'\n", code.c_str());
            VisIt::Execute(sim,code);
        }
        
        
        
        
        static inline
        void setup_callbacks(void *cbdata) throw()
        {
            VisItSetSlaveProcessCallback(SlaveProcessCallback);
            VisItSetCommandCallback(ControlCommandCallback,cbdata);
            //VisItSetGetMetaData(SimGetMetaData,cbdata);
            //VisItSetGetMesh(SimGetMesh, cbdata);
            //VisItSetGetDomainList(SimGetDomainList,cbdata);
            //VisItSetGetVariable(SimGetVariable,cbdata);
            //VisItSetGetCurve(SimGetCurve,cbdata);
        }
    }
    
    void VisIt:: Loop(Simulation &sim)
    {
        
        //______________________________________________________________________
        //
        //
        // preparing loop
        //
        //______________________________________________________________________
        const mpi &MPI = sim.MPI;
        MPI.Printf0(stderr, "[VisIt] Start Loop\n");
        
        
        //______________________________________________________________________
        //
        //
        // file descriptor for the console
        //
        //______________________________________________________________________
        const int  fd         = sim.console ? fileno(stdin) : -1;
        if( sim.console )
        {
            MPI.Printf0(stderr, "[VisIt] \tConsole is Active\n");
        }
        
        
        //void *cbdata     = &sim; // passed on every callbacks
        
        //______________________________________________________________________
        //
        //
        // running
        //
        //______________________________________________________________________
        while(!sim.done)
        {
            //__________________________________________________________________
            //
            // Get input from VisIt or timeout so the simulation can run.
            //__________________________________________________________________
            const int blocking   = ( sim.runMode == VISIT_SIMMODE_RUNNING) ? 0 : 1;
            int       visitstate =   -1;
            if(sim.is_first)
            {
                visitstate = VisItDetectInput(blocking,fd);
            }
            
            //__________________________________________________________________
            //
            // broadcast status from rank=0
            //__________________________________________________________________
            MPI.Bcast(visitstate,0,MPI_COMM_WORLD);
            
            MPI.Printf0(stderr, "VisIt State=%d\n", visitstate);
            
            //__________________________________________________________________
            //
            // Now, every one acts depending on visitstate
            //__________________________________________________________________
            switch(visitstate)
            {
                    
                case 0:
                    //__________________________________________________________
                    //
                    // There was no input from VisIt, return control to sim.
                    //__________________________________________________________
                    MPI.Printf0(stderr,"-- Perform one step...");
                    break;
                    
                case 1:
                    //__________________________________________________________
                    //
                    // VisIt is trying to connect !
                    //__________________________________________________________
                    MPI.Printf0(stderr,"[VisIt] Connecting...\n");
                    if( VisItAttemptToCompleteConnection() )
                    {
                        MPI.Printf0(stderr, "[VisIt] \tConnected\n");
                        // setup visit callbacks
                        setup_callbacks( &sim );
                        
                        // stop simulation upon connection
                        sim.runMode = VISIT_SIMMODE_STOPPED;
                        
                        // request metadata
                        VisItTimeStepChanged();
                    }
                    else
                    {
                        MPI.Printf0(stderr, "[VisIt] \tConnection Failure: %s\n", VisItGetLastError() );
                    }
                    
                case 2:
                    //__________________________________________________________
                    //
                    // VisIt is sending a command
                    //__________________________________________________________
                    if(!ProcessVisItCommand(sim))
                    {
                        MPI.Printf0(stderr, "[VisIt] Disconnected\n");
                        VisItDisconnect();
                    }
                    break;
                    
                    
                case 3:
                    //__________________________________________________________
                    //
                    // VisIt console input
                    //__________________________________________________________
                    ProcessConsole(sim);
                    break;
                    
                case -1:
                case -2:
                case -3:
                case -4:
                case -5:
                    throw exception("VisItDetectInput Error %d",visitstate);
                    break;
                    
                    
                default:{
                    const yocto::exception excp("VisitDetectInput: unknown VisIt State=%d", visitstate);
                    MPI.Printf0(stderr,"%s", excp.when());
                    throw excp;
                }
            }
        }
        VisItDisconnect();
        MPI.Printf0(stderr, "[VisIt] End Loop\n");
        
    }
    
}