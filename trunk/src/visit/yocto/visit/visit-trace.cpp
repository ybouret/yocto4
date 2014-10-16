#include "yocto/visit/visit.hpp"


namespace yocto
{
    
    VisIt::TraceFile:: TraceFile( const mpi &MPI, const string &filename ) :
    rank_(MPI.CommWorldRank)
    {
        initialize(filename);
    }
    
    
    VisIt::TraceFile:: TraceFile( const mpi &MPI, const char *filename ) :
    rank_(MPI.CommWorldRank)
    {
        const string fn(filename);
        initialize(fn);
    }
    
    VisIt:: TraceFile:: ~TraceFile() throw()
    {
        if( 0 == rank_ )
            VisItCloseTraceFile();
    }

    void VisIt:: TraceFile:: initialize(const string &filename)
    {
        if( 0 == rank_)
            VisItOpenTraceFile( filename.c_str() );
    }
    
    

}