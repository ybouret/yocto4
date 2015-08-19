#include "yocto/parallel/split.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace parallel
    {
        patch1D split::compute(size_t rank,size_t size,const patch1D &source)
        {
            unit_t offset = source.lower;
            unit_t length = source.width;
            split::in1D(rank, size, offset, length);
            if(length<=0) throw exception("parallel.split produced empty patch!");
            return patch1D(offset,offset+length-1);
        }

    }
    
}
