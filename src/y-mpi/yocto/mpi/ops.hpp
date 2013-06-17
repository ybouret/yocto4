#ifndef YOCTO_MPI_OPS_INCLUDED
#define YOCTO_MPI_OPS_INCLUDED 1

#include "yocto/mpi/mpi.hpp"


namespace yocto
{
    
    struct mpi_ops
    {
        //! Allreduce on one type
        static void apply(const mpi &  MPI,
                          void        *output,
                          const void  *input,
                          MPI_Datatype datatype,
                          MPI_Op       op,
                          MPI_Comm     comm
                          );
        
        //! templated operation on one type
        template <typename T,MPI_Op   OP> static inline
        T apply(const mpi &MPI,
                const T    input,
                MPI_Comm   comm)
        {
            T output(0);
            mpi_ops::apply(MPI,
                           &output,
                           &input,
                           MPI.get_type<T>(),
                           OP,
                           comm);
            return output;
        }
        
        //! specialized sum
        template <typename T> static inline
        T sum(const mpi &MPI, T x, MPI_Comm comm = MPI_COMM_WORLD)
        {
            return apply<T,MPI_SUM>(MPI,x,comm);
        }
        
        //! specialized prod
        template <typename T> static inline
        T prod(const mpi &MPI, T x, MPI_Comm comm = MPI_COMM_WORLD)
        {
            return apply<T,MPI_PROD>(MPI,x,comm);
        }
        
        //! specialized min
        template <typename T> static inline
        T min(const mpi &MPI, T x, MPI_Comm comm = MPI_COMM_WORLD)
        {
            return apply<T,MPI_MIN>(MPI,x,comm);
        }
        
        //! specialized min
        template <typename T> static inline
        T max(const mpi &MPI, T x, MPI_Comm comm = MPI_COMM_WORLD)
        {
            return apply<T,MPI_MAX>(MPI,x,comm);
        }
        
        
        
    };
    
}

#endif

