#include "yocto/visit/interface.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
    
    visit:: ~visit() throw() {}
    
    
    const char visit::name[] = "VisIt";
    
    namespace 
    {
        static int  *   visit_argc        = NULL;
        static char *** visit_argv        = NULL;
        static const char *   visit_sim_name    = NULL;
        static const char *   visit_sim_comment = NULL;
        static const char *   visit_sim_path    = NULL;
        
        static int visit_broadcast_int_callback(int *value, int sender)
        {
            return MPI_Bcast(value, 1, MPI_INT, sender, MPI_COMM_WORLD);
        }
        
        static int visit_broadcast_string_callback(char *str, int len,
                                                   int sender)
        {
            return MPI_Bcast(str, len, MPI_CHAR, sender, MPI_COMM_WORLD);
        }
        
        static inline void visit_cleanup() throw()
        {
            visit_argc        = NULL;
            visit_argv        = NULL;
            visit_sim_name    = NULL;
            visit_sim_comment = NULL;
            visit_sim_path    = NULL;
        }
        
    }
    
    visit:: visit( ) :
    rank(0),
    size(0),
    is_parallel(false),
    is_master(false)
    {
        
        if(!visit_argc        || 
           !visit_argv        || 
           !visit_sim_name    || 
           !visit_sim_comment || 
           !visit_sim_path)
        {
            throw exception("Can't call VisIt instance directly");
        }
        
        //----------------------------------------------------------------------
        // initialize visit
        //----------------------------------------------------------------------
        VisItSetupEnvironment();
        
        
        //----------------------------------------------------------------------
        // initialize MPI
        //----------------------------------------------------------------------
        mpi & MPI = mpi::init( visit_argc, visit_argv );
        (int&)rank = MPI.CommWorldRank;
        (int&)size = MPI.CommWorldSize;
        (bool&)is_parallel = size  > 1;
        (bool&)is_master   = 0 == rank;
        
        //----------------------------------------------------------------------
        // Install callback for communication
        //----------------------------------------------------------------------
        VisItSetBroadcastIntFunction(visit_broadcast_int_callback); 
        VisItSetBroadcastStringFunction(visit_broadcast_string_callback);
        
        //----------------------------------------------------------------------
        // Tell libsim whether the simulation is parallel
        //----------------------------------------------------------------------
        VisItSetParallel(is_parallel); 
        VisItSetParallelRank(rank);
        
        if( is_master )
        {
            if( !VisItInitializeSocketAndDumpSimFile(visit_sim_name,
                                                     visit_sim_comment,
                                                     visit_sim_path,
                                                     NULL,
                                                     NULL, 
                                                     NULL) )
            {
                throw exception("VisItInitializeSocketAndDumpSimFile failure!");
            }
        }
        
        visit_cleanup();
    }
    
    
    visit & visit:: SetupEnvironment(int *         argc, 
                                     char ***      argv,
                                     const string &sim_name,
                                     const string &sim_comment,
                                     const string &sim_path )
    {
        
        try 
        {
            visit_argc        = argc;
            visit_argv        = argv;
            visit_sim_name    = sim_name.c_str();
            visit_sim_comment = sim_comment.c_str();
            visit_sim_path    = sim_path.c_str(); 
            return visit:: instance();
        }
        catch(...)
        {
            visit_cleanup();
            throw;
        }
    }
    
}
