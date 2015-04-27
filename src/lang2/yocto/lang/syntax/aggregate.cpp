#include "yocto/lang/syntax/aggregate.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/exception.hpp"

#include <iostream>

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            aggregate:: ~aggregate() throw()
            {
            }

            aggregate:: aggregate(const string &id) :
            logical(id)
            {

            }


            bool aggregate:: accept(Y_LANG_SYNTAX_RULE_ACCEPT_ARGS) const
            {
                std::cerr << "?AGG '" << label << "'" << std::endl;

                if(operands.size<=0)
                    throw exception("aggregate '%s' has no operand", label.c_str());

                auto_ptr<xnode> agg( xnode::leaf(*this) );

                for(const operand *node=operands.head;node;node=node->next)
                {
                    const rule &r   = *(node->addr);
                    xnode      *sub = 0;
                    if(!r.accept(sub, lxr, src, fp))
                    {
                        assert(0==sub);
                        xnode::restore(agg.yield(), lxr);
                        return false;
                    }
                    
                    if(sub)
                    {
                        agg->append(sub);
                    }
                }
                
                grow(tree,agg.yield());
                return true;
            }
        }
    }
}

