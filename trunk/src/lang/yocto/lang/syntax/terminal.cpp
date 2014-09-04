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
            
            terminal:: terminal(const string &id) :
            rule(id)
            {
            }
            
            terminal * terminal:: create(const string &id)
            {
                return new terminal(id);
            }
            
            YOCTO_LANG_SYNTAX_RULE_MATCH_IMPL(terminal)
            {
                lexeme *lx = Lexer.get(Source, Input);
                if(lx)
                {
                    if(lx->label==label)
                    {
                        xtree Node = xnode::create(label,lx);
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
