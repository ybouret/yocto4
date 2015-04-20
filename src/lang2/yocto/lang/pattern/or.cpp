#include "yocto/lang/pattern/logic.hpp"

namespace yocto
{
    namespace lang
    {
        OR:: ~OR() throw() {}


        OR:: OR() throw() : logical( OR::UUID ) {}

        OR:: OR(const OR &other) : logical(other) {}

        pattern *OR:: clone() const
        {
            return new OR(*this);
        }

        bool OR:: match(Y_LANG_PATTERN_MATCH_ARGS) const
        {

            // does anybody match ?
            for(const pattern *p = operands.head;p;p=p->next)
            {
                if( p->match(tkn, src, fp) )
                {
                    return true;
                }
            }

            //nobody matches
            return false;
        }

        logical *OR::create()
        {
            return new OR();
        }
        
    }
}
