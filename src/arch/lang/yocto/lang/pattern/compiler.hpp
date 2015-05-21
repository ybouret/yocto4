#ifndef YOCTO_LANG_PATTERN_COMPILER_INCLUDED
#define YOCTO_LANG_PATTERN_COMPILER_INCLUDED 1

#include "yocto/lang/pattern/dict.hpp"

namespace yocto
{
    namespace lang
    {
        
        pattern *compile( const string &expr, const p_dict *dict = 0);
        pattern *compile( const char   *expr, const p_dict *dict = 0);

    }
}


#endif


