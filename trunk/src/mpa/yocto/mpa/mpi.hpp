#ifndef YOCTO_MPA_MPI_INCLUDED
#define YOCTO_MPA_MPI_INCLUDED 1

#include "yocto/mpi/mpi.hpp"
#include "yocto/mpa/rational.hpp"
#include "yocto/memory/buffers.hpp"

namespace yocto
{
    namespace mpa
    {
        
        struct mpi_io
        {
            static const int tag = 7;
            //==================================================================
            //
            // natural I/O
            //
            //==================================================================
            inline static
            natural recv_n(const mpi &MPI,
                           int        source,
                           MPI_Comm   comm )
            {
                MPI_Status   status;
                const size_t length =  MPI.Recv<size_t>(source, tag, comm, status);
                if(length)
                {
                    memory::buffer_of<char,memory::global> buf(length);
                    MPI.Recv(buf.rw(), length, MPI_BYTE, source, tag, comm, status);
                    mpn ans(buf);
                    return ans;
                }
                else
                {
                    return mpn();
                }
            }
            
            inline static
            void send(const mpi     &MPI,
                      const natural &n,
                      int            dest,
                      MPI_Comm       comm)
            {
                const size_t length = n.length();
                MPI.Send<size_t>(length, dest, tag, comm);
                if(length)
                {
                    MPI.Send(n.ro(), length, MPI_BYTE, dest, tag, comm);
                }
            }
        };
        
    }
    
}


#endif
