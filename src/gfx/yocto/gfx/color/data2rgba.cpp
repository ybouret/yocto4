#include "yocto/gfx/color/data2rgba.hpp"

namespace yocto
{
    namespace gfx
    {
        data2rgba::  data2rgba() throw() {}
        data2rgba:: ~data2rgba() throw() {}

        RGBA data2rgba:: operator()(const void *addr)
        {
            return get(addr);
        }

    }

}
