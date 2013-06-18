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
        template <typename T> static inline
        T apply(const mpi &MPI,
                const T    input,
		MPI_Op     op,
                MPI_Comm   comm)
        {
            T output(0);
            mpi_ops::apply(MPI,
                           &output,
                           &input,
                           MPI.get_type<T>(),
                           op,
                           comm);
            return output;
        }
        
        //! specialized sum
        template <typename T> static inline
        T sum(const mpi &MPI, T x, MPI_Comm comm = MPI_COMM_WORLD)
        {
            return apply<T>(MPI,x,MPI_SUM,comm);
        }
        
        //! specialized prod
        template <typename T> static inline
        T prod(const mpi &MPI, T x, MPI_Comm comm = MPI_COMM_WORLD)
        {
            return apply<T>(MPI,x,MPI_PROD,comm);
        }
        
        //! specialized min
        template <typename T> static inline
        T min(const mpi &MPI, T x, MPI_Comm comm = MPI_COMM_WORLD)
        {
            return apply<T>(MPI,x,MPI_MIN,comm);
        }
        
        //! specialized min
        template <typename T> static inline
        T max(const mpi &MPI, T x, MPI_Comm comm = MPI_COMM_WORLD)
        {
            return apply<T>(MPI,x,MPI_MAX,comm);
        }
        
        
        
    };
    
}

#endif

