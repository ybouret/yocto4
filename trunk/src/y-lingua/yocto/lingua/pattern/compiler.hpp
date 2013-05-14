#ifndef YOCTO_LINGUA_PATTERN_COMPILER_INCLUDED
#define YOCTO_LINGUA_PATTERN_COMPILER_INCLUDED 1


#include "yocto/lingua/pattern/dict.hpp"

namespace yocto
{
    namespace lingua
    {
        
        pattern *compile( const string &expr, const p_dict *dict = 0);
        pattern *compile( const char   *expr, const p_dict *dict = 0);
        
    }
}

#endif
