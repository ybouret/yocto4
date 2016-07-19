#include "yocto/gfx/ops/edges.hpp"

namespace yocto
{
    namespace gfx
    {
        edges:: edges(const unit_t W, const unit_t H) :
        pixmap<float>(W,H),
        I(W,H)
        {
        }

        edges:: ~edges() throw()
        {

        }

        const vertex edges::shift[9] =
        {
            vertex(-1,-1), vertex(-1,0), vertex(-1,1),
            vertex( 0,-1), vertex( 0,0), vertex( 0,1),
            vertex( 1,-1), vertex( 1,0), vertex( 1,1)
        };

        const float edges::sobel1[9] =
        {
            1,0,-1,
            2,0,-2,
            1,0,-1
        };

        const float edges::sobel2[9] =
        {
            1,2,1,
            0,0,0,
            -1,-2,-1
        };

    }
}
