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

            terminal:: terminal( const string &id ) :
            rule(id)
            {
            }

            bool terminal:: accept(Y_LANG_SYNTAX_RULE_ACCEPT_ARGS) const
            {
                lexeme *lx = lxr.get(src, fp);
                if(!lx)
                {
                    return false; //EOF
                }
                else
                {
                    if(lx->label==label)
                    {
                        grow(tree, xnode::term(*this,lx));
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
                fp << "[shape=box,label=\"" << label << "\"];\n";
            }

            void terminal:: lnk(ios::ostream &) const {}

        }
        
    }
    
}

