#include "yocto/gfx/ops/filter.hpp"

namespace yocto
{
    namespace gfx
    {
        _filter:: ~_filter() throw()
        {
        }

        _filter:: _filter() throw()
        {
        }

        const vertex _filter:: shift[ _filter::items ] =
        {
            vertex(-1,-1), vertex(0,-1), vertex(1,-1),
            vertex(-1, 0), vertex(0, 0), vertex(1, 0),
            vertex(-1, 1), vertex(0, 1), vertex(1, 1)
        };

    }
}

