
#include "yocto/lang/grammar.hpp"
#include "yocto/exception.hpp"
#include "yocto/ptr/auto.hpp"

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
                if( Lexer.is_active(Source) && Lexer.peek()->label != __eof )
                {
                    const lexeme *lx = Lexer.peek();
                    throw exception("%s: illegal extraneous token '%s' (line %u)", name.c_str(), lx->label.c_str(), unsigned(lx->line) );
                }
                std::cerr << "[[ SUCCESS ]]" << std::endl;
                return ans.yield();
            }
            
            //==================================================================
            // syntax error
            //==================================================================
            
            std::cerr << "[[ FAILURE ]]" << std::endl << std::endl;
            
            const lexeme *lx_last = Lexer.get(-1);
            const lexeme *lx_prev = Lexer.get(-2);
            const char   *id      = name.c_str();
            if(lx_last )
            {
                exception    excp("%s: invalid <%s>",id, lx_last->label.c_str());
                {
                    const string str_last( lx_last->to_string() );
                    if( str_last.size() )
                        excp.cat("='%s'", str_last.c_str());
                }
                excp.cat(" (line %u)", unsigned(lx_last->line) );
                if( lx_prev )
                {
                    excp.cat(" after <%s>", lx_prev->label.c_str() );
                    {
                        const string str_prev( lx_prev->to_string() );
                        if( str_prev.size() )
                            excp.cat("='%s'", str_prev.c_str() );
                    }
                    excp.cat(" (line %u)", unsigned(lx_prev->line) );
                }
                throw excp;
            }
            else
            {
                throw exception("%s: unexpected no lexeme!",id);
            }
            return NULL;
        }
        
    }
    
}
