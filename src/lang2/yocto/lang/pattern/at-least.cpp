#include "yocto/lang/pattern/joker.hpp"

namespace yocto
{
    namespace lang
    {
        at_least:: ~at_least() throw() {}

        at_least:: at_least(pattern *p, size_t n) throw() :
        joker(UUID, p),
        count(n)
        {}

        pattern *at_least:: create(pattern *p, size_t n)
        {
            assert(p);
            try
            {
                return new at_least(p,n);
            }
            catch(...)
            {
                delete p;
                throw;
            }

        }


        pattern *at_least:: clone() const
        {
            return create( jk->clone(), count);
        }

        bool at_least:: match(Y_LANG_PATTERN_MATCH_ARGS) const
        {
            token stk;
            size_t num = 0;
            while(jk->match(stk,src,fp))
            {
                ++num;
            }
            if(num>=count)
            {
                tkn.merge_back(stk);
                return true;
            }
            else
            {
                src.unget(stk);
                return false;
            }
        }
    }
}
