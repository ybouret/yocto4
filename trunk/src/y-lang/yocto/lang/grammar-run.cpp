
#include "yocto/lang/grammar.hpp"
#include "yocto/exception.hpp"
#include "yocto/auto-ptr.hpp"

namespace yocto 
{
    namespace lang 
    {
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // grammar accept
        //
        ////////////////////////////////////////////////////////////////////////
        syntax::parse_node *grammar:: accept( lexer &Lexer, regex::source &Source )
        {
            if( rules.size <= 0 )
                throw exception("empty grammar '%s'", name.c_str() );
            
            syntax::parse_node *Tree = NULL;
            if( rules.head->match(Lexer,Source,Tree) ) 
            {
                auto_ptr<syntax::parse_node> ans( Tree );
                if( Lexer.is_active(Source) )
                {
                    const lexeme *lx = Lexer.peek();
                    throw exception("%s: illegal extraneous token '%s'", name.c_str(), lx->label.c_str() );
                }
                std::cerr << "[[ SUCCESS ]]" << std::endl;
                return ans.yield();
            }
            
            //==================================================================
            // syntax error
            //==================================================================
            
            std::cerr << "[[ FAILURE ]]" << std::endl << std::endl;
            const lexeme *lx =  Lexer.last();
            if( lx )
            {
                std::cerr << name << ":" << lx->line << ": invalid " << lx->label;
                if( lx->size > 0 )
                    std::cerr << "='" << *lx << "'";
                delete Lexer.prev_lexeme();
                if( NULL != (lx=Lexer.last()) )
                {
                    std::cerr << " after " << lx->label;
                    if( lx->size > 0 )
                        std::cerr << "='" << *lx << "'";
                    std::cerr << "@line " << lx->line;
                }
                std::cerr << std::endl;
            }
            
            return NULL;
        }
        
    }
    
}
