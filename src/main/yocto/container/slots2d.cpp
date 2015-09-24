#include "yocto/container/slots2d.hpp"
#include "yocto/exception.hpp"
#include "yocto/code/round.hpp"

namespace yocto
{
    slots2D:: ~slots2D() throw()
    {
    }

    slots2D:: slots2D(const size_t nr,
                      const size_t nc,
                      const size_t item_size,
                      const size_t row_bytes):
    rows(nr),
    cols(nc),
    items(nr*nc),
    shift( memory::align(row_bytes*rows) ),
    cmem(shift+item_size*items)
    {
        assert(nr>0);
        assert(nc>0);
        assert(item_size>0);
    }


    size_t slots2D:: bytes() const throw()
    {
        return cmem.size;
    }

}

