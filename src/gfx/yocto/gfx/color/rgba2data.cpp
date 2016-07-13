#include "yocto/gfx/color/rgba2data.hpp"

namespace yocto
{
    namespace gfx
    {

        rgba2data:: rgba2data() throw()
        {
        }

        rgba2data:: ~rgba2data() throw()
        {
        }


        void rgba2data:: operator()(void *addr, const RGBA &c)
        {
            put(addr,c);
        }


    }

}
