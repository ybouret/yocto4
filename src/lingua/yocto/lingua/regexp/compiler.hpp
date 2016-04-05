#ifndef YOCTO_LINGUA_REGEXP_COMPILER_INCLUDED
#define YOCTO_LINGUA_REGEXP_COMPILER_INCLUDED 1

#include "yocto/lingua/pattern/dict.hpp"

namespace yocto
{
    namespace lingua
    {
        struct regexp
        {
            static pattern *compile(const string &expr,const p_dict *dict=0);
            static pattern *compile(const char   *expr,const p_dict *dict=0);
        };
    }
}

#endif
