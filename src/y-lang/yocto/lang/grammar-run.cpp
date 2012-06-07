
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
            
            syntax::context     Context;
            syntax::parse_node *Tree = NULL;
            if( rules.head->match(Lexer,Source,Tree,Context) ) 
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
            
            std::cerr << "[[ FAILURE ]]" << std::endl;
            const lexeme       *lx = Context.lx;
            const syntax::rule *r  = Context.calling;
            if( r )
            {
                std::cerr << "During call to <" << r->label << ">" << std::endl;
            }
            else
            {
                std::cerr << "Undefined calling rule !" << std::endl;
            }
            if( lx )
            {
                std::cerr << "last terminal= " << lx->label << " '"<< *lx << "', @line " << lx->line << std::endl;
            }
            else 
            {
                std::cerr << "EMPTY source" << std::endl;
            }
            return NULL;
        }
        
    }
    
}
