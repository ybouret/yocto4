#ifndef YOCTO_MPL_MPI_INCLUDED
#define YOCTO_MPL_MPI_INCLUDED 1

#include "yocto/mpi/mpi.hpp"
#include "yocto/mpl/natural.hpp"

namespace yocto
{
    namespace mpl
    {

#define YOCTO_MPL_MPI_TAG 5

        inline
        void send(const mpi     &MPI,
                  const natural &x,
                  const int      dest,
                  MPI_Comm       comm )
        {
            const uint32_t count = x.length();
            MPI.Send(count, dest, YOCTO_MPL_MPI_TAG, comm);
            MPI.Send(x.ro(),count,MPI_BYTE,dest, YOCTO_MPL_MPI_TAG,comm);
        }

        inline
        natural recv(const mpi &MPI,
                     const int from,
                     MPI_Comm comm)
        {
            MPI_Status     status;
            const uint32_t count = MPI.Recv<uint32_t>(from, YOCTO_MPL_MPI_TAG, comm, status);
            natural        n(count,as_capacity);
            MPI.Recv((void*)(n.ro()), count, MPI_BYTE,from, YOCTO_MPL_MPI_TAG,comm,status);
            n.update();
            return n;
        }
    }
}

#endif
