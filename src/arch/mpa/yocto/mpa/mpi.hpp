#ifndef YOCTO_MPA_MPI_INCLUDED
#define YOCTO_MPA_MPI_INCLUDED 1

#include "yocto/mpi/mpi.hpp"
#include "yocto/mpa/rational.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/memory/pooled.hpp"

namespace yocto
{
    namespace mpa
    {
        
        struct mpi_io
        {
            static const int tag = 9;
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
                    memory::buffer_of<char,memory::pooled> buf(length);
                    MPI.Recv(buf.rw(), length, MPI_BYTE, source, tag, comm, status);
                    natural ans(buf);
                    return ans;
                }
                else
                {
                    return natural();
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
            
            //==================================================================
            //
            // integer I/O
            //
            //==================================================================
            inline static
            integer recv_z(const mpi &MPI,
                           int        source,
                           MPI_Comm   comm )
            {
                MPI_Status    status;
                const int8_t  s = MPI.Recv<int8_t>(source,tag,comm,status);
                const natural n = recv_n(MPI,source,comm);
                integer       z(n);
                if(s<0)
                {
                    z.neg();
                }
                return z;
            }
            
            inline static
            void send(const mpi     &MPI,
                      const integer &z,
                      int            dest,
                      MPI_Comm       comm)
            
            {
                int8_t s = 0;
                switch(z.s)
                {
                    case __positive:
                        s = 1;
                        break;
                    case __zero:
                        break;
                    case __negative:
                        s = -1;
                        break;
                }
                MPI.Send<int8_t>(s,dest,tag,comm);
                mpi_io::send(MPI,z.n,dest,comm);
            }
            
            
            //==================================================================
            //
            // rational I/O
            //
            //==================================================================
            static inline
            rational recv_q(const mpi &MPI,
                            int        source,
                            MPI_Comm   comm )
            {
                const integer num = recv_z(MPI,source,comm);
                const natural den = recv_n(MPI,source,comm);
                return mpq(num,den);
            }
            
            static inline
            void send(const mpi     &MPI,
                      const rational &q,
                      int            dest,
                      MPI_Comm       comm)
            
            {
                send(MPI,q.num,dest,comm);
                send(MPI,q.den,dest,comm);
            }
        };
        
    }
    
}


#endif
