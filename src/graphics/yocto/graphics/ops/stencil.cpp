#include "yocto/graphics/ops/stencil.hpp"
#include <cstring>

namespace yocto
{

    namespace graphics
    {

        const vertex _stencil::shift[9] =
        {
            vertex(-1,-1), vertex(-1,0), vertex(-1,1),
            vertex( 0,-1), vertex( 0,0), vertex( 0,1),
            vertex( 1,-1), vertex( 1,0), vertex( 1,1)
        };


    }

}