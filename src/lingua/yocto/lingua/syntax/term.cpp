#include "yocto/lingua/syntax/term.hpp"
#include "yocto/ios/graphviz.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            terminal:: ~terminal() throw()
            {
            }

            terminal:: terminal(const string &id) :
            rule(id,UUID)
            {
                YOCTO_LINGUA_PATTERN_IS(terminal);
            }


            void terminal:: viz(ios::ostream &fp) const
            {
                const rule *addr = this;
                fp.viz(addr);
                fp << "[label=\"";
                ios::graphviz_encode(label,fp);
                fp << "\",shape=box";
                switch(flags)
                {
                    case property::standard: fp << ",style=solid";    break;
                    case property::univocal: fp << ",style=rounded"; break;
                    case property::jettison: fp << ",style=dotted";  break;
                }

                fp << "];\n";
            }


            bool terminal:: admit(YOCTO_LINGUA_SYNTAX_RULE_ADMIT_ARGS) const
            {
                lexeme *lx = lxr.get(src);
                if(!lx)
                {
                    return false; // End Of Input
                }
                else
                {
                    if(label==lx->label)
                    {
                        xnode *node = xnode::create(*this,lx);
                        grow(tree,node);
                        return true;
                    }
                    else
                    {
                        lxr.unget(lx);
                        return false;
                    }
                }
            }

            bool terminal:: admit_empty() const throw()
            {
                return false;
            }

        }
    }
}

