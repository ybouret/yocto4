#include "yocto/lang/regexp/compiler.hxx"
#include "yocto/ptr/auto.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {

        pattern *regexp( const string &expr, const p_dict *dict )
        {
            RegExp rx(expr.c_str(),expr.size(),dict);

            auto_ptr<pattern> p(rx.parse_expr());
            if(rx.level>0)
            {
                throw exception("unfinished expression '%s'", expr.c_str());
            }
            return p.yield();
        }

        pattern *regexp( const char *expr, const p_dict *dict )
        {
            const  string s(expr);
            return regexp(s,dict);
        }
        
    }
    
}
