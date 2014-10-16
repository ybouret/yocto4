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
        
        
        
        // a command from the gui...
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
            char        *cmd = sim.iobuff(0);
            const size_t len = sim.iobuff.length();
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
            MPI.Printf(stderr, "[VisIt::Console] '%s'\n", code.c_str());
            VisIt::Execute(sim,code);
        }
        
        
        //______________________________________________________________________
        //
        //
        // SimGetMetaData callback
        //
        //______________________________________________________________________
        static visit_handle SimGetMetaData(void *cbdata)
        {
            assert(cbdata!=NULL);
            visit_handle       md  = VISIT_INVALID_HANDLE;
            VisIt::Simulation &sim = *(VisIt::Simulation *)cbdata;
            
            
            if( VisIt_SimulationMetaData_alloc(&md) == VISIT_OKAY)
            {
                assert( VISIT_INVALID_HANDLE != md );
                
                /* Meta Data for Simulation */
                VisIt_SimulationMetaData_setMode(md,sim.runMode);
                VisIt_SimulationMetaData_setCycleTime(md,sim.cycle,sim.runTime);
                
                /* Create Generic Interface/Commands */
                for(register size_t i = 0; i <  VisIt::Simulation::GenericCommandNum; ++i)
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
        
        
        //______________________________________________________________________
        //
        //
        // SimGetMesh callback
        //
        //______________________________________________________________________
        static inline
        visit_handle SimGetMesh( int domain, const char *name, void *cbdata )
        {
            assert(cbdata);
            VisIt::Simulation &sim = *(VisIt::Simulation *)cbdata;
            const string mesh_name(name);
            return sim.get_mesh( domain, mesh_name );
        }
        
        
        //______________________________________________________________________
        //
        //
        // SimGetVariable callback
        //
        //______________________________________________________________________
        static
        visit_handle SimGetVariable(int domain, const char *name, void *cbdata)
        {
            assert(cbdata);
            VisIt::Simulation &sim = *(VisIt::Simulation *)cbdata;
            const string       id(name);
            return sim.get_variable(domain, id);
        }

        //______________________________________________________________________
        //
        //
        // SimGetCurve callback
        //
        //______________________________________________________________________
        static inline
        visit_handle SimGetCurve( const char *name, void *cbdata )
        {
            VisIt::Simulation &sim = *(VisIt::Simulation *)cbdata;
            const string       id(name);
            return sim.get_curve(id);
        }

        
        static inline void setup_callbacks(void *cbdata) throw()
        {
            VisItSetSlaveProcessCallback(SlaveProcessCallback);
            VisItSetCommandCallback(ControlCommandCallback,cbdata);
            VisItSetGetMetaData(SimGetMetaData,cbdata);
            VisItSetGetMesh(SimGetMesh, cbdata);
            //VisItSetGetDomainList(SimGetDomainList,cbdata);
            VisItSetGetVariable(SimGetVariable,cbdata);
            VisItSetGetCurve(SimGetCurve,cbdata);
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
                    VisIt::OneStep(sim);
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