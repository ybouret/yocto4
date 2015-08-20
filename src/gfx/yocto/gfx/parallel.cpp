#include "yocto/gfx/parallel.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace gfx
    {
#if 0
        void ipatch:: setup_parallel_metrics(unit_t &xoff,
                                             unit_t &yoff,
                                             unit_t &xlen,
                                             unit_t &ylen,
                                             const unit_t w,
                                             const unit_t h,
                                             const bool   full)
        {
            xoff = 0;
            yoff = 0;
            xlen = w;
            ylen = h;
            if(!full)
            {
                xoff =  1;
                yoff =  1;
                xlen -= 2;
                ylen -= 2;
            }
            if(xlen<0||ylen<0)
            {
                throw exception("setup_parallel_metrics: too small dimensions");
            }
        }

        ipatch:: ipatch(const rectangle &r) throw() :
        rect(r),
        source(0),
        target(0),
        params(0)
        {}

        ipatch:: ~ipatch() throw() {}
#endif
        
    }

}
