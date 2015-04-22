#include "yocto/lang/regexp/compiler.hxx"

namespace yocto
{
    namespace lang
    {

        pattern *regexp( const string &expr, const p_dict *dict )
        {
            RegExp rx(expr.c_str(),expr.size(),dict);

            return rx.parse_expr();
        }
        
    }
    
}

