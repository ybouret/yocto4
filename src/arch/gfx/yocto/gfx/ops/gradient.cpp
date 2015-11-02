#include "yocto/gfx/ops/gradient.hpp"

namespace yocto
{
    namespace gfx
    {

        gradient::ipatch:: ~ipatch() throw() {}
        gradient::ipatch::  ipatch(const patch2D &p) throw() :
        patch(p),
        Gmax(0),
        target(0),
        source(0)
        {}


    }
}

namespace yocto
{
    namespace gfx
    {

        gradient::opatch:: ~opatch() throw() {}
        gradient::opatch::  opatch(const patch2D &p) throw() :
        patch(p),
        Gmax(0),
        target(0),
        source(0)
        {}
    }
}

