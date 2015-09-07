#ifndef YOCTO_PARALLEL_BASIC_INCLUDED
#define YOCTO_PARALLEL_BASIC_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
    namespace parallel
    {

        template <typename T> inline
        static  void basic_split(const size_t rank,
                                 const size_t size,
                                 T           &offset,
                                 T           &length) throw()
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
