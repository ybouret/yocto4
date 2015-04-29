#include "yocto/lang/pattern/logic.hpp"

namespace yocto
{
    namespace lang
    {
        NOT:: ~NOT() throw() {}


        NOT:: NOT() throw() : logical( NOT::UUID ) {}

        NOT:: NOT(const NOT &other) : logical(other) {}

        pattern *NOT:: clone() const
        {
            return new NOT(*this);
        }

        bool NOT:: match(Y_LANG_PATTERN_MATCH_ARGS) const
        {


            token stk;

            //! does anyone match ?
            for(const pattern *p=operands.head;p;p=p->next)
            {
                if(p->match(stk,src,fp))
                {
                    src.unget(stk);
                    return false;
                }
            }
            
            // nobody matched...
            if(!src.peek(fp))
            {
                // EOF => false
                return false;
            }
            else
            {
                // a not matching char...
                tkn.push_back( src.get(fp) );
                return true;
            }
        }

        logical *NOT::create()
        {
            return new NOT();
        }
        
        void NOT:: viz(ios::ostream &fp) const
        {
            fp.viz((const pattern *)this);
            fp("[shape=diamond,label=\"!!\"];\n");
            vizops(fp);
        }

        bool NOT:: accept_empty() const throw()
        {
            //! if returns something, at least one char..
            return false;
        }

        
    }
}
