#include "yocto/graphics/ops/tagmap.hpp"

namespace yocto
{
    namespace graphics
    {
        tagmap:: ~tagmap() throw()
        {

        }

        tagmap:: tagmap( const unit_t W, const unit_t H) :
        pixmap<size_t>(W,H),
        current(0),
        v_stack(512,as_capacity),
        to_rgba()
        {
        }

        void tagmap:: initialize() throw()
        {
            ldz();
            current=0;
        }

    }

}
