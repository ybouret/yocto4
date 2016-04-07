#ifndef YOCTO_LINGUA_PATTERN_REGEXP_INCLUDED
#define YOCTO_LINGUA_PATTERN_REGEXP_INCLUDED 1

#include "yocto/lingua/pattern/dict.hpp"

namespace yocto
{
    namespace lingua
    {
        pattern *regexp(const string &expr,const p_dict *dict=0);
        pattern *regexp(const char   *expr,const p_dict *dict=0);
    }
}

#endif
