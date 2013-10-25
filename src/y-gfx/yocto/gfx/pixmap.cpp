#include "yocto/gfx/pixmap.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace gfx
    {
        void pixmap_base:: check_same( size_t Depth, size_t SizeOf )
        {
            if(Depth!=SizeOf)
                throw exception("Incompatible Depth=%u/SizeOf=%u", unsigned(Depth), unsigned(SizeOf));
        }
    }
}
