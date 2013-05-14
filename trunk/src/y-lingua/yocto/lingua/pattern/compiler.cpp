#include "yocto/lingua/pattern/compiler.hpp"

namespace yocto
{
    namespace lingua
    {
        
        pattern *compile( const string &expr, p_dict *dict )
        {
            return 0;
        }
        
        pattern *compile( const char *expr, p_dict *dict )
        {
            const string xp(expr);
            return compile(xp,dict);
        }
        
    }
}
