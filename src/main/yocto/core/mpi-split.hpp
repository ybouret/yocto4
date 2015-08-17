#ifndef YOCTO_CODE_MPI_SPLIT_INCLUDED
#define YOCTO_CODE_MPI_SPLIT_INCLUDED 1

#include "yocto/os.hpp"


namespace yocto
{
    namespace core
    {
        template <typename T>
        inline void mpi_split(size_t rank,
                              size_t size,
                              T     &offset,
                              T     &length) throw()
        {
            assert(size>0);
            assert(rank<size);
            //assert(length>=0);
            T todo(length/size);
            for(size_t r=1;r<=rank;++r)
            {
                length -= todo;
                offset += todo;
                todo    = length/(size-r);
            }
            length = todo;
        }


        extern void find_mpi_split(size_t      &px,
                                   size_t      &py,
                                   const size_t nproc,
                                   const size_t Nx,
                                   const size_t Ny);

        template <typename T>
        inline void mpi_split(size_t rank,
                              size_t size,
                              T &xoffset,
                              T &yoffset,
                              T &xlength,
                              T &ylength)
        {

            const size_t Nx = xlength;
            const size_t Ny = ylength;
            // find the number of procs per dimension
            size_t       px = 0;
            size_t       py = 0;
            find_mpi_split(px,py,size,Nx,Ny);

            // find the local ranks
            const size_t rx = rank % px;
            const size_t ry = rank / px;
            //std::cerr << "px=" << px << ", py=" << py << ", rx=" << rx << ", ry=" << ry << std::endl;
            mpi_split(rx,px,xoffset,xlength);
            mpi_split(ry,py,xoffset,ylength);
        }
        
        
    }
}

#endif
