#include "yocto/gfx/parallel.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace gfx

    {
        void patch:: setup_parallel_metrics(coord2D     &offset,
                                            coord2D     &length,
                                            const unit_t w,
                                            const unit_t h,
                                            const bool   full)
        {
            offset.x = 0;
            offset.y = 0;
            length.x = w;
            length.y = h;
            if(!full)
            {
                offset.x =  1;
                offset.y =  1;
                length.x -= 2;
                length.y -= 2;
            }
            if(length.x<=0||length.y<=0)
            {
                throw exception("setup_parallel_metrics: too small dimensions");
            }
        }

        patch:: ~patch() throw() {}

        patch:: patch( const patch2D &p ) throw() :
        area(p.lower.x,p.lower.y,p.width.x,p.width.y)
        {
        }

        
    }
    
}
