#include "yocto/visit/visit.hpp"


namespace yocto
{
    
    void VisIt:: SetDirectory( const string &path )
    {
        VisItSetDirectory(  (char *) path.c_str() );
    }
    
    void VisIt:: SetOption( const string &options )
    {
        VisItSetOptions( (char *) options.c_str() );
    }
    
    
    namespace
    {
        static inline
        int visit_broadcast_int_callback(int *value,
                                         int  sender)
        {
            return MPI_Bcast(value, 1, MPI_INT, sender, MPI_COMM_WORLD);
        }
        
        static inline
        int visit_broadcast_string_callback(char *str,
                                            int   len,
                                            int   sender)
        {
            return MPI_Bcast(str, len, MPI_CHAR, sender, MPI_COMM_WORLD);
        }
    }
    
    
    //__________________________________________________________________________
    //
    //
    // Setup functions
    //
    //__________________________________________________________________________
    void VisIt:: SetupParallel(const mpi    &MPI ,
                               const string &sim_name,
                               const string &sim_comment,
                               const string &sim_path,
                               const string *sim_ui)
    {
        MPI.Printf( stderr, "[Visit] SetupParallel %d.%d\n", MPI.CommWorldSize, MPI.CommWorldRank);
        
        
        //----------------------------------------------------------------------
        // Install callback functions for global communication.
        //----------------------------------------------------------------------
        VisItSetBroadcastIntFunction(visit_broadcast_int_callback);
        VisItSetBroadcastStringFunction(visit_broadcast_string_callback);
        
        //----------------------------------------------------------------------
        // Tell libsim whether the simulation is parallel.
        //----------------------------------------------------------------------
        VisItSetParallel(     MPI.CommWorldSize > 1 );
        VisItSetParallelRank( MPI.CommWorldRank     );
        
        //----------------------------------------------------------------------
        // Finish setting up the simulation
        //----------------------------------------------------------------------
        const char *ui_file = sim_ui ? sim_ui->c_str() : NULL;
        MPI.Printf0( stderr, "[Visit] sim name    = '%s'\n", sim_name.c_str()    );
        MPI.Printf0( stderr, "[Visit] sim comment = '%s'\n", sim_comment.c_str() );
        MPI.Printf0( stderr, "[Visit] sim path    = '%s'\n", sim_path.c_str()    );
        if( ui_file )
        {
            MPI.Printf0( stderr, "[Visit] UI file     = '%s'\n", ui_file );
        }
        
        //----------------------------------------------------------------------
        // startup VisIt, version 2
        //----------------------------------------------------------------------
        char *env = 0;
        if( MPI.IsFirst )
        {
            env = VisItGetEnvironment();
            //if( !env ) throw exception("VisItGetEnvironment(FAILURE)");
        }
        
        if( !VisItSetupEnvironment2(env) )
        {
            //if(env) free(env); throw exception("VisItSetupEnvironment2(FAILURE)");
        }
        
        if(env) free(env);
        
        
        if(  MPI.IsFirst )
        {
            if( ! VisItInitializeSocketAndDumpSimFile(sim_name.c_str(),
                                                      sim_comment.c_str(),
                                                      sim_path.c_str(),
                                                      NULL,
                                                      ui_file,
                                                      NULL
                                                      ) )
                throw exception("VisItInitializeSocketAndDumpSimFile");
        }
    }


    //__________________________________________________________________________
    //
    //
    // executing one step of simulation
    //
    //__________________________________________________________________________
    void VisIt:: OneStep(Simulation &sim)
    {
        ++sim.cycle;
        const double enter = MPI_Wtime();
        sim.step();
        sim.ellapsed = MPI_Wtime() - enter;
        
        VisItTimeStepChanged();
        VisItUpdatePlots();
    }
    
    
}