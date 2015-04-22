#ifndef YOCTO_LANG_REGEXP_INCLUDED
#define YOCTO_LANG_REGEXP_INCLUDED 1

#include "yocto/lang/pattern/dict.hpp"


namespace yocto
{
    namespace lang
    {

        pattern *regexp( const string &expr, const p_dict *dict );

    }
}

#endif
