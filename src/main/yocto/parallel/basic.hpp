#ifndef YOCTO_PARALLEL_BASIC_INCLUDED
#define YOCTO_PARALLEL_BASIC_INCLUDED 1

#include "yocto/os.hpp"
#include <cmath>

namespace yocto
{
    namespace parallel
    {

        template <typename T> inline
        void basic_split(const size_t rank,
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

        inline
        double efficiency(const double speed_up,const size_t nthreads)
        {
            if(nthreads<=1)
            {
                return 100.0;
            }
            else
            {
                return floor(10000.0*(speed_up-1.0)/(nthreads-1))/100.0;
            }
        }
    }
}

#endif
