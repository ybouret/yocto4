#include "yocto/gfx/region.hpp"

namespace yocto
{
    namespace gfx
    {
        
        region:: ~region() throw() {}
        
        region:: region( unit_t X, unit_t Y, size_t W, size_t H ) throw():
        x(X),
        y(Y),
        w(W),
        h(H),
        xend(x+w),
        yend(y+h)
        {
        }
        
        region::region( const region &other ) throw() :
        x(other.x),
        y(other.y),
        w(other.w),
        h(other.h),
        xend(other.xend),
        yend(other.yend)
        {
            
        }
        
    }
}