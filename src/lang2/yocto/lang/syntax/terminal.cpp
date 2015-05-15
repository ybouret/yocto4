#include "yocto/lang/syntax/terminal.hpp"

#include <iostream>

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            terminal:: ~terminal() throw()
            {
            }

            terminal:: terminal( const string &id, const property ppty ) :
            rule(id,UUID),
            modifier(ppty)
            {
            }

            bool terminal:: accept(Y_LANG_SYNTAX_RULE_ACCEPT_ARGS) const
            {
                lexeme *lx = lxr.get(src, fp);
                if(!lx)
                {
                    return false; //EOF or stopped
                }
                else
                {
                    if(lx->label==label)
                    {
                        grow(tree, xnode::term(*this,lx,modifier));
                        return true;
                    }
                    else
                    {
                        lxr.unget(lx);
                        return false;
                    }
                }
            }

            void terminal::viz( ios::ostream &fp ) const
            {
                fp.viz((const rule*)this);
                const char *shape = "box";

                if(univocal==modifier)
                {
                    shape = "circle";
                }

                if(jettison==modifier)
                {
                    shape = "circle,style=filled";
                }

                fp("[shape=%s,label=\"",shape);
                fp << label << "\"];\n";
            }

            void terminal:: lnk(ios::ostream &) const {}

        }
        
    }
    
}

