#ifndef YOCTO_STRING_XNUM_INCLUDED
#define YOCTO_STRING_XNUM_INCLUDED 1

namespace yocto
{
    template <>
    struct xnumeric<string>
    {
        inline static string zero() { return string(); }
    };

}

#endif
