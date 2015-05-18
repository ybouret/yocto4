#include "yocto/lang/syntax/alternate.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/exception.hpp"

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
            logical( make_label(), UUID)
            {

            }


            bool alternate:: accept(Y_LANG_SYNTAX_RULE_ACCEPT_ARGS) const
            {

                if(size<=0)
                    throw exception("alternate '%s' has no operand", label.c_str());

                for(const operand *node=head;node;node=node->next)
                {
                    xnode *sub = 0;
                    if( node->addr->accept(sub, lxr, src, fp))
                    {
                        grow(tree, sub);
                        return true;
                    }
                }

                return false;
            }

            void alternate:: viz(ios::ostream &fp) const
            {
                fp.viz((const rule*)this); fp("[shape=diamond,label=\"");
                encode_label(fp);
                fp("\"];\n");
            }

            void alternate:: lnk( ios::ostream &fp ) const
            {
                for(const operand *node=head;node;node=node->next)
                {
                    fp.viz( (const rule*)this ); fp << " -> "; fp.viz(node->addr); fp << ";\n";
                }
            }
            
        }
    }
}

