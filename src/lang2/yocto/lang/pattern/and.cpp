#include "yocto/lang/pattern/logic.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {
        AND:: ~AND() throw() {}


        AND:: AND() throw() : logical( AND::UUID ) {}

        AND:: AND(const AND &other) : logical(other) {}

        pattern *AND:: clone() const
        {
            return new AND(*this);
        }

        bool AND:: match(Y_LANG_PATTERN_MATCH_ARGS) const
        {
            if(operands.size<=0) throw exception("AND: no operands");

            token stk;

            // does everybody match ?
            for(const pattern *p = operands.head;p;p=p->next)
            {
                if( !p->match(stk, src, fp) )
                {
                    src.unget(stk);
                    return false;
                }
            }
            
            //everyone matches
            tkn.merge_back(stk);
            return true;
        }

        logical *AND::create()
        {
            return new AND();
        }

    }
}
