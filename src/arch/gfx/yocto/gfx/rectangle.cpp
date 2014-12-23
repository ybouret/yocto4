#include "yocto/gfx/rectangle.hpp"

namespace yocto
{
    namespace gfx
    {
        
        rectangle:: rectangle(unit_t xorg, unit_t yorg, size_t width, size_t height) throw() :
        x(xorg),
        y(yorg),
        w(width),
        h(height),
        xout(x+w),
        yout(y+h)
        {
        }
        
        rectangle:: rectangle(const rectangle &other ) throw() :
        x(other.x),
        y(other.y),
        w(other.w),
        h(other.h),
        xout(other.xout),
        yout(other.yout)
        {
        }
        
        rectangle:: ~rectangle() throw()
        {
        }
        
    }
}
