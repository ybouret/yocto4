#ifndef YOCTO_LANG_REGEXP_INCLUDED
#define YOCTO_LANG_REGEXP_INCLUDED 1

#include "yocto/lang/pattern/dict.hpp"


namespace yocto
{
    namespace lang
    {

        //! regular expression compiler
        pattern *regexp( const string &expr, const p_dict *dict );
        
        //! regular expression compiler
        pattern *regexp( const char   *expr, const p_dict *dict );

    }
}

#endif
