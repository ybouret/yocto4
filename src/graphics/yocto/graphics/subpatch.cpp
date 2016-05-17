#include "yocto/graphics/subpatch.hpp"

namespace yocto
{
    namespace graphics
    {
        subpatch:: subpatch(const patch &p) :
        patch(p),
        xslot()
        {
        }

        subpatch:: ~subpatch() throw()
        {
        }
        
    }
}
