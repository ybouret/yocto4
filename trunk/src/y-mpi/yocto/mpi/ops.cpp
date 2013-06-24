#include "yocto/mpi/ops.hpp"

namespace yocto
{
    
    void mpi:: Allreduce1(void        *output,
                          const void  *input,
                          MPI_Datatype datatype,
                          MPI_Op       op,
                          MPI_Comm     comm) const
    {
        assert(output);
        assert(input);
        Allreduce(input, output, 1, datatype, op, comm);
    }
    
    void mpi_ops:: apply(const yocto::mpi &MPI,
                         void             *output,
                         const void       *input,
                         MPI_Datatype      datatype,
                         MPI_Op            op,
                         MPI_Comm          comm)
    {
        assert(output);
        assert(input);
        MPI.Allreduce(input, output, 1, datatype, op, comm);
    }
    
}