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
                              T     &length)
        {
            assert(size>0);
            assert(rank<size);
            T todo(length/size);
            for(size_t r=1;r<=rank;++r)
            {
                length -= todo;
                offset += todo;
                todo    = length/(size-r);
            }
            length = todo;
        }
    }
}

#endif
