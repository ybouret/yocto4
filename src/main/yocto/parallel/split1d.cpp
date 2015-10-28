#include "yocto/parallel/split1d.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/parallel/basic.hpp"

namespace yocto
{
    namespace parallel
    {
        split1D:: ~split1D() throw()
        {
        }

        split1D:: split1D(const size_t nprocs, const patch1D &p ) :
        patch1D(p),
        cores( clamp<size_t>(1,nprocs,width) )
        {
            
        }

        patch1D split1D:: operator()(size_t rank) const
        {
            assert(rank<cores);
            unit_t start = lower;
            unit_t count = width;
            basic_split(rank, cores, start, count); assert(count>0);
            return patch1D(start,start+count-1);
        }


        split1D:: split1D(const split1D &other) :
        patch1D(other), cores(other.cores)
        {
        }

    }

}
