#include "yocto/graphics/rgba2data.hpp"

namespace yocto
{
    namespace graphix
    {

        rgba2data:: ~rgba2data() throw()
        {
        }


        rgba2data:: rgba2data() throw()
        {
        }

        void rgba2data:: operator()(void *addr, const RGBA &c)
        {
            assert(addr);
            put(addr,c);
        }

    }

}

