#include "yocto/lang/compiler.hpp"

namespace yocto 
{
    namespace  lang 
    {
        
        compiler:: ~compiler() throw()
        {
            
        }
        
        compiler:: compiler() :
        parser( "lang.compiler.lexer", "compiler.grammar" )
        {
            
            //------------------------------------------------------------------
            // scanner specifics
            //------------------------------------------------------------------
            scan.make( "BLANKS", "[ \t]+",    & scan.discard );
            scan.make( "ENDL"  , "[:endl:]+", & scan.newline );
            
            //------------------------------------------------------------------
            // declare terminals
            //------------------------------------------------------------------
            syntax::terminal &RULEID   = terminal( "RULEID", "[:word:]+" );
            syntax::terminal &REGEXP   = terminal( "REGEXP", "[:cstring:]");
            syntax::terminal &COLUMN   = terminal( ":",         ":", syntax::is_discardable );
            syntax::terminal &END      = terminal( ";",         ";", syntax::is_discardable );
            //syntax::terminal &LPAREN   = terminal( "LPAREN",    "\\(", syntax::is_discardable );
            //syntax::terminal &RPAREN   = terminal( "RPAREN",    "\\)", syntax::is_discardable );
            //syntax::terminal &MODIFIER = terminal( "MODIFIER", "[*+?]" ); 
            //syntax::terminal &ALT      = terminal( "ALT",      "\\|", syntax::is_discardable);
            syntax::aggregate &RULE  = agg( "RULE" );
            //------------------------------------------------------------------
            // Rule prolog
            //------------------------------------------------------------------
            RULE << RULEID << COLUMN;
            
                      
            //------------------------------------------------------------------
            // Rule body
            //------------------------------------------------------------------
            RULE &= REGEXP;
            
            //------------------------------------------------------------------
            // Rule epilog
            //------------------------------------------------------------------
            RULE &= END;
            
            //------------------------------------------------------------------
            // and the grammar
            //------------------------------------------------------------------
            syntax::repeating &GRAMMAR = rep("RULES",RULE,1);
            set_root(GRAMMAR);
        }
        
        
        syntax::parse_node * compiler:: operator()( ios::istream &fp )
        {
            regex::source       Source( fp );
            reset();
            syntax::parse_node *Tree = accept(*this, Source);
            Tree->AST();
            return Tree;
        }
        
        
                
    }
    
}