#include "yocto/mpi/mpi.hpp"

namespace yocto
{
    mpi:: comm_world:: comm_world( const mpi &ref ) :
    MPI( ref ),
    size(     MPI.CommWorldSize ),
    rank(     MPI.CommWorldRank ),
    last(     MPI.CommWorldLast ),
    first(    MPI.IsFirst       ),
    final(    MPI.IsFinal       ),
    parallel( MPI.IsParallel    )
    {
    }
    
    mpi:: comm_world:: ~comm_world() throw() {}
    
    
}

