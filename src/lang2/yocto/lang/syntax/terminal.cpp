#include "yocto/lang/syntax/terminal.hpp"

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

            bool terminal:: accept(Y_LANG_SYNTAX_RULE_ACCEPT_ARGS)
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
                        grow( tree, xnode::term(*this,lx) );
                        return true;
                    }
                    else
                    {
                        lxr.unget(lx);
                        return false;
                    }
                }
            }
            
        }
        
    }
    
}

