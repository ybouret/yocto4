#ifndef YOCTO_MATH_XTYPES_INCLUDED
#define YOCTO_MATH_XTYPES_INCLUDED 1

#include "yocto/type/args.hpp"

namespace yocto
{

    //! default, must be superseeded for C++ types, aka rational...
    template <typename T>
    struct xnumeric
    {
        YOCTO_ARGUMENTS_DECL_T;
        inline static T zero() throw() { return T(0); }
        inline static T one()  throw() { return T(1); }
    };


}

#endif
