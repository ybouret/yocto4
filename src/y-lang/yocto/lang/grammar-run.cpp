
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
                    throw exception("%u: illegal token '%s'", lx->label.c_str() );
                }
                std::cerr << "[[ SUCCESS ]]" << std::endl;
                return ans.yield();
            }
            
            //==================================================================
            // syntax error
            //==================================================================
            
            std::cerr << "[[ FAILURE ]]" << std::endl;
            return NULL;
        }
        
    }
    
}
