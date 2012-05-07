#include "yocto/visit/interface.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
    
    
    
    VisIt:: TraceFile:: TraceFile( int rank, const string &filename  ) : 
    rank_(rank)
    {
        if( 0 == rank_)
            VisItOpenTraceFile( filename.c_str() );
    }
    
    
    VisIt:: TraceFile:: ~TraceFile() throw()
    {
        if( 0 == rank_ )
            VisItCloseTraceFile();
    }
    
    void VisIt:: SetDirectory( const string &path )
    {
        VisItSetDirectory(  (char *) path.c_str() );
    }
    
    void VisIt:: SetOption( const string &options )
    {
        VisItSetOptions( (char *) options.c_str() );
    }
    
    void VisIt:: SetupEnvironment()
    {
        if( !VisItSetupEnvironment() )
            throw exception("VisItSetupEnvironment Failure!");
    }
    
    namespace 
    {
        static int visit_broadcast_int_callback(int *value, int sender)
        {
            return MPI_Bcast(value, 1, MPI_INT, sender, MPI_COMM_WORLD);
        }
        static int visit_broadcast_string_callback(char *str, int len,
                                                   int sender)
        {
            return MPI_Bcast(str, len, MPI_CHAR, sender, MPI_COMM_WORLD);
        }
    }
    
    void VisIt:: SetupParallel(const mpi    &MPI , 
                               const string &sim_name,
                               const string &sim_comment,
                               const string &sim_path,
                               const string *sim_ui)
    {
        MPI.Printf0( stderr, "[Visit] SetupParallel\n");
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
        
        if( 0 == MPI.CommWorldRank )
        {
            
            const char *ui_file = sim_ui ? sim_ui->c_str() : NULL;
            MPI.Printf0( stderr, "[Visit] sim name    = '%s'\n", sim_name.c_str() );
            MPI.Printf0( stderr, "[Visit] sim comment = '%s'\n", sim_comment.c_str() );
            MPI.Printf0( stderr, "[Visit] sim path    = '%s'\n", sim_path.c_str() );
            if( ui_file )
                MPI.Printf0( stderr, "[Visit] UI file     = '%s'\n", ui_file );
            if( ! VisItInitializeSocketAndDumpSimFile(sim_name.c_str(), 
                                                      sim_comment.c_str(), 
                                                      sim_path.c_str(),
                                                      NULL, 
                                                      ui_file,
                                                      NULL
                                                      ) )
                throw exception("VisItInitialize Failure: %s", VisItGetLastError() );
        }
    }
}
