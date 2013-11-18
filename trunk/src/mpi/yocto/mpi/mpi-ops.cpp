#include "yocto/mpi/mpi.hpp"

namespace yocto
{
	
    //! Allreduce on one type
    void mpi:: Allreduce1(void        *output,
                          const void  *input,
                          MPI_Datatype datatype,
                          MPI_Op       op,
                          MPI_Comm     comm
                          ) const
    {
        assert(output);
        assert(input);
        
        Allreduce(input, output, 1, datatype, op, comm);
        
    }
    
    
}
