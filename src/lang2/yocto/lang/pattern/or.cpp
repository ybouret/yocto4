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
        
        void OR:: viz(ios::ostream &fp) const
        {
            fp.viz((const pattern *)this);
            fp("[shape=diamond,label=\"||\"];\n");
            vizops(fp);
        }

        bool OR:: accept_empty() const throw()
        {
            //! if one of the pattern accepts empty => true
            for(const pattern *p = operands.head;p;p=p->next)
            {
                if(p->accept_empty())
                {
                    return true;
                }
            }
            return false;
        }

    }
}
