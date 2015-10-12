#ifndef YOCTO_STRING_XNUM_INCLUDED
#define YOCTO_STRING_XNUM_INCLUDED 1

#include "yocto/string.hpp"
namespace yocto
{
    template <>
    struct xnumeric<string>
    {
        inline static string zero() { return string(); }
    };

}

#endif
