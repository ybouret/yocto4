#ifndef YOCTO_SPADE_MPI_INCLUDED
#define YOCTO_SPADE_MPI_INCLUDED 1

#include "yocto/spade/dataspace.hpp"
#include "yocto/mpi/mpi.hpp"

namespace yocto
{
    namespace spade
    {
        struct _mpi
        {
            
            
            template <typename DATASPACE>
            static void init_exchange( const mpi & MPI, DATASPACE &ds, mpi::Requests requests )
            {
                
            }
            
            
            
        };
    }
}

#endif
