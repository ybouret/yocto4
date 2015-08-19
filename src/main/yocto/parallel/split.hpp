#ifndef YOCTO_PARALLEL_SPLIT_INCLUDED
#define YOCTO_PARALLEL_SPLIT_INCLUDED 1

#include "yocto/parallel/patch.hpp"

namespace yocto
{
    namespace parallel
    {
        struct split
        {
            template <typename T> inline
            static  void in1D(size_t rank,
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

            static patch1D compute(size_t rank,size_t size,const patch1D &source);
            
        };
    }
}

#endif
