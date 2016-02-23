#ifndef YOCTO_MPL_MPI_INCLUDED
#define YOCTO_MPL_MPI_INCLUDED 1

#include "yocto/mpi/mpi.hpp"
#include "yocto/mpl/rational.hpp"

namespace yocto
{
    namespace mpl
    {

#define YOCTO_MPL_MPI_TAG 5
        template <typename T>
        void send(const mpi &,
                  const T   &,
                  const int,
                  MPI_Comm);

        template <typename T>
        T recv(const mpi &MPI, const int from, MPI_Comm comm);

        template <>
        inline
        void send<natural>(const mpi     &MPI,
                           const natural &x,
                           const int      dest,
                           MPI_Comm       comm )
        {
            const uint32_t count = x.length();
            MPI.Send(count, dest, YOCTO_MPL_MPI_TAG, comm);
            MPI.Send(x.ro(),count,MPI_BYTE,dest, YOCTO_MPL_MPI_TAG,comm);
        }


        template <>
        inline
        natural recv<natural>(const mpi &MPI,
                              const int from,
                              MPI_Comm comm)
        {
            MPI_Status     status;
            const uint32_t count = MPI.Recv<uint32_t>(from, YOCTO_MPL_MPI_TAG,comm,status);
            natural        n(count,as_capacity);
            MPI.Recv((void*)(n.ro()), count, MPI_BYTE,from, YOCTO_MPL_MPI_TAG,comm,status);
            n.update();
            return n;
        }

        template <>
        inline
        void send<integer>(const mpi     &MPI,
                           const integer &z,
                           const int      dest,
                           MPI_Comm       comm )
        {
            const int8_t s = int8_t(z.s);
            MPI.Send<int8_t>(s, dest, YOCTO_MPL_MPI_TAG, comm);
            send<natural>(MPI,z.n,dest,comm);
        }

        template <>
        inline
        integer recv<integer>(const mpi &MPI,
                              const int from,
                              MPI_Comm  comm)
        {
            MPI_Status status;
            sign_type s = int2sign(MPI.Recv<int8_t>(from, YOCTO_MPL_MPI_TAG, comm, status));
            natural   n = recv<natural>(MPI,from,comm);
            return integer(s,n);
        }
        
        
        
        
    }
}

#endif
