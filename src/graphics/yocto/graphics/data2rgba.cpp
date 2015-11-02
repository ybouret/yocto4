#include "yocto/graphics/data2rgba.hpp"

namespace yocto
{
    namespace graphix
    {
        data2rgba:: ~data2rgba() throw() {}
        
        
        data2rgba:: data2rgba() throw() {}
        
        RGBA data2rgba:: operator()(const void *addr)
        {
            assert(addr);
            return get(addr);
        }

    }
}

