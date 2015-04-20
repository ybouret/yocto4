#include "yocto/lang/pattern/dict.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {
        p_dict :: p_dict() throw(): pattern_map() {}

        p_dict:: ~p_dict() throw()
        {
        }

        void p_dict:: define(const string &id, pattern *p)
        {
            const pattern_ptr q(p);
            if(!insert(id,q))
                throw exception("p_dict: multiple '%s'", id.c_str());
        }

        void p_dict:: define( const char   *id, pattern *p )
        {
            const pattern_ptr q(p);
            const string      ID(id);
            if(!insert(ID,q))
                throw exception("p_dict: multiple '%s'", ID.c_str());
        }


        
    }
}
