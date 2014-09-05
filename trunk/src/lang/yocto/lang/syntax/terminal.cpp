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
            
            terminal:: terminal(const string &id, const xnode_ppty ppty) :
            rule(id),
            semantic(ppty)
            {
            }
            
          
            
            YOCTO_LANG_SYNTAX_RULE_MATCH_IMPL(terminal)
            {
                check(Tree);
                lexeme *lx = Lexer.get(Source, Input);
                if(lx)
                {
                    if(lx->label==label)
                    {
                        xtree Node = xnode::create(label,lx,is_regular);
                        grow(Tree,Node);
                        return true;
                    }
                    else
                    {
                        Lexer.unget(lx);
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
            
            
        }
        
    }
    
}
