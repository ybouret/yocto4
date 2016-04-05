#include "yocto/lingua/pattern/dict.hpp"
#include "yocto/exception.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace lingua
    {
        p_dict ::  p_dict() throw() : pattern_db() {}
        p_dict :: ~p_dict() throw() {}


        void p_dict :: define(const string &id, pattern *p)
        {
            const pattern_ptr q(p);
            if( ! insert(id,q) )
            {
                throw exception("lingua::p_dict.multiple('%s')", id.c_str());
            }
        }

        void p_dict:: define(const char *id, pattern *p)
        {
            auto_ptr<pattern> q(p);
            const string      ID(id);
            define(ID,q.yield());
        }

        pattern * p_dict:: operator[](const string &id) const
        {
            const pattern_ptr *pp = search(id);
            if(!pp)
            {
                throw exception("undeclared pattern '%s'", id.c_str());
            }
            return (*pp)->clone();
        }

        pattern * p_dict:: operator[](const char *id) const
        {
            const string ID(id);
            return (*this)[ID];
        }

    }
}
