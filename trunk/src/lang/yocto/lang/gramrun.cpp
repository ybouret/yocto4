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
                //______________________________________________________________
                //
                // syntax error...
                //______________________________________________________________
                const lexeme *last = Lexer.cache_tail();
                switch( Lexer.cache_size() )
                {
                    case 0:
                        throw exception("%d: ((%s)) unknown syntax error", Lexer.line, name.c_str());
                        
                    case 1:
                        assert(last);
                    {
                        const string content1 = last->to_string();
                        throw exception("%d: ((%s)) unexpected %s '%s'", last->line, name.c_str(), last->label.c_str(), content1.c_str() );
                    }
                        break;
                        
                    default:
                        break;
                }
                assert(last);
                assert(last->prev);
                const lexeme *prev    = last->prev;
                const string  content = last->to_string();
                const string  prev_content = prev->to_string();
                throw exception("%d: ((%s)) unexpected %s '%s' after %s '%s' (line %d)",
                                last->line,
                                name.c_str(),
                                last->label.c_str(),
                                content.c_str(),
                                prev->label.c_str(),
                                prev_content.c_str(),
                                prev->line);
                
                
            }
            
            //__________________________________________________________________
            //
            // other syntax error: TODO set a stop flag....
            //__________________________________________________________________

            auto_ptr<syntax::xnode> TreeGuard(Tree);
            const lexeme *lx = Lexer.peek(Source, Input);
            if(lx)
            {
                throw exception("%d: ((%s)): unexpected extra %s '%s'",
                                lx->line,
                                name.c_str(),
                                Lexer.last_label.c_str(),
                                Lexer.last_token.c_str());
            }
            
            return TreeGuard.yield();
            
        }
        
        
    }
    
}
