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
            syntax::terminal &LPAREN   = terminal( "LPAREN",    "\\(", syntax::is_discardable );
            syntax::terminal &RPAREN   = terminal( "RPAREN",    "\\)", syntax::is_discardable );
            syntax::terminal &MODIFIER = terminal( "MODIFIER", "[*+?]" ); 
            syntax::terminal &ALT      = terminal( "ALT",      "\\|", syntax::is_discardable);
            syntax::aggregate &RULE  = agg( "RULE" );
            //------------------------------------------------------------------
            // Rule prolog
            //------------------------------------------------------------------
            RULE << RULEID << COLUMN;
            
                      
            //------------------------------------------------------------------
            // Rule content
            //------------------------------------------------------------------
            syntax::aggregate &ELEMENT = agg("ELEMENT");
            syntax::repeating &ELEMENTS = rep("ELEMENTS",ELEMENT,1);
            syntax::alternate &ATOM = alt("ATOM");
            ATOM |= RULEID;
            ATOM |= REGEXP;
            
            syntax::alternate &ALT_EXPR = alt("ALT_EXPR");
            ALT_EXPR |= ATOM;
            syntax::aggregate &EXPR = agg("EXPR");
            EXPR << LPAREN << ELEMENTS << RPAREN;
            ALT_EXPR |= EXPR;
            
            ELEMENT &= ALT_EXPR;
            ELEMENT &= opt("OPT_MODIFIER",MODIFIER);
            
            syntax::aggregate &ALTERNATE = agg("ALTERNATE");
            ALTERNATE << ALT << ELEMENTS;
            
            RULE &= ELEMENTS;
            RULE &= rep("OTHER_ELEMENTS",ALTERNATE,0);
            
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