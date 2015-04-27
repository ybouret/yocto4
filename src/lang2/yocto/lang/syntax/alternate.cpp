#include "yocto/lang/syntax/alternate.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/exception.hpp"

#include <iostream>

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            unsigned alternate:: counter = 0;

            string alternate:: make_label()
            {
                YOCTO_GIANT_LOCK();
                ++counter;
                const string ans = vformat("ALT#%u",counter);
                return ans;
            }

            alternate:: ~alternate() throw()
            {
            }

            

            alternate:: alternate() :
            logical( make_label() )
            {

            }


            bool alternate:: accept(Y_LANG_SYNTAX_RULE_ACCEPT_ARGS) const
            {
                std::cerr << "?ALT " << label << std::endl;

                if(operands.size<=0)
                    throw exception("alternate '%s' has no operand", label.c_str());

                for(const operand *node=operands.head;node;node=node->next)
                {
                    xnode *sub = 0;
                    if( node->addr->accept(sub, lxr, src, fp))
                    {
                        if(sub) grow(tree, sub);
                        return true;
                    }
                }

                return false;
            }
        }
    }
}

