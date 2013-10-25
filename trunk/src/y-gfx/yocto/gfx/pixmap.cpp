#include "yocto/gfx/pixmap.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace gfx
    {
        
        void pixmap_check_same( size_t depth, size_t sz )
        {
            assert(depth>0);
            assert(sz>0);
            
            if(depth!=sz)
                throw exception("Invalid bitmap depth=%u with sizeof(T)=%u",
                                unsigned(depth),
                                unsigned(sz));
        }
    }

}
