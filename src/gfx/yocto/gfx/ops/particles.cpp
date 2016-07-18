#include "yocto/gfx/ops/particles.hpp"

namespace yocto
{

    namespace gfx
    {
        tagmap:: tagmap(const unit_t W, const unit_t H) :
        _tagmap(W,H),
        current(0),
        vstk()
        {
        }


        tagmap:: ~tagmap() throw()
        {
            
        }

    }

}
