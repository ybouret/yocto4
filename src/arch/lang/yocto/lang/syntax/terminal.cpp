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
            
            terminal:: terminal(const string &id, const property ppty) :
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
                        //std::cerr << " ==> TERM '" << label << "'" << std::endl;
                        xtree Node = xnode::create(label,lx,semantic);
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
