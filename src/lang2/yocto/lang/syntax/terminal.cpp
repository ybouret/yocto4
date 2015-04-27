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
                std::cerr << "?TERM '" << label << "'" << std::endl;
                lexeme *lx = lxr.get(src, fp);
                if(!lx)
                {
                    return false; //EOF
                }
                else
                {
                    if(lx->label==label)
                    {
                        std::cerr << "\t+" << label << std::endl;
                        grow(tree, xnode::term(*this,lx));
                        return true;
                    }
                    else
                    {
                        std::cerr << "\t-" << label << std::endl;
                        lxr.unget(lx);
                        return false;
                    }
                }
            }
            
        }
        
    }
    
}

