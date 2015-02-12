#ifndef YOCTO_GEMS_MPI_INCLUDED
#define YOCTO_GEMS_MPI_INCLUDED 1

#include "yocto/gems/atoms.hpp"
#include "yocto/mpi/mpi.hpp"

namespace yocto
{
    namespace gems
    {

        struct mpi_io
        {
            static const int tag = 8;

            //! send an encoded bunch of atoms
            template <typename T>
            static inline void send( const mpi &MPI, const binary_atoms<T> &data, int rank )
            {
                assert(MPI.CommWorldRank!=rank);
                const size_t n = data.size;
                MPI.Send<uint64_t>(n, rank, tag, MPI_COMM_WORLD);
                if(n>0)
                {
                    MPI.Send(data.ro(), data.length(), MPI_BYTE, rank, tag, MPI_COMM_WORLD);
                }
            }

            template <typename T>
            static inline void recv( const mpi &MPI,  binary_atoms<T> &data, int rank )
            {
                assert(MPI.CommWorldRank!=rank);
                MPI_Status status;
                const size_t n = size_t(MPI.Recv<uint64_t>(0, tag, MPI_COMM_WORLD, status));
                if(n>0)
                {
                    data.build(n);  // ensure memory
                    data.assume(n); // set active memory
                    MPI.Recv(data.rw(),data.length(),MPI_BYTE,rank,tag,MPI_COMM_WORLD,status);
                }
            }
        };

    }
}

#endif
