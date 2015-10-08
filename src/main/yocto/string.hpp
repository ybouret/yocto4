#ifndef YOCTO_STRING_INCLUDED
#define YOCTO_STRING_INCLUDED 1


#include "yocto/string/basic.hpp"
#include "yocto/code/printf-check.hpp"
#include "yocto/bitwise.hpp"
#include "yocto/math/xtypes.hpp"

namespace yocto
{
    typedef basic::string<char>   string;
    typedef basic::string<utf8_t> wstring;

    string vformat( const char *fmt,... ) YOCTO_PRINTF_CHECK(1,2);
}

YOCTO_SUPPORT_BITWISE_OPS(string)
YOCTO_SUPPORT_BITWISE_OPS(wstring)

namespace yocto
{
    template <>
    struct xnumeric<string>
    {
        inline static string zero() { return string(); }
    };

}

#endif
