#include "yocto/lang/grammar.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {
        syntax::xtree grammar:: accept( lexer &Lexer, source &Source, ios::istream &Input )
        {
            if(rules.size<=0)
                throw exception("((%s)) has no rules", name.c_str());
            
            syntax::xtree Tree = 0;
            syntax::rule *Root = rules.head; assert(Root);
            if(!Root->match(Lexer,Source,Input,Tree))
            {
                throw exception("((%s)) can't accept", name.c_str());
            }
            
            auto_ptr<syntax::xnode> SafeTree(Tree);
            const lexeme *lx = Lexer.peek(Source, Input);
            if(lx)
            {
                throw exception("%d: ((%s)): unexpected %s '%s' after root %s",
                                Lexer.line,
                                name.c_str(),
                                Lexer.last_label.c_str(),
                                Lexer.last_token.c_str(),
                                Root->label.c_str());
            }
            
            return SafeTree.yield();
            
        }
        
        
    }
    
}
