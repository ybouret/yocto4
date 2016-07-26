#include "yocto/gfx/ops/tagmap.hpp"

namespace yocto
{

    namespace gfx
    {
        tagmap:: tagmap(const unit_t W, const unit_t H) :
        _tagmap(W,H),
        current(0),
        vstk(),
	colors()
        {
        }


        tagmap:: ~tagmap() throw()
        {

        }

    }

}
