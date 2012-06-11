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
            syntax::terminal  &RULEID   = terminal( "RULEID", "[:word:]+" );
            syntax::terminal  &REGEXP   = terminal( "REGEXP", "[:cstring:]");
            syntax::terminal  &SINGLE   = terminal( "SINGLE", "'[\\x20-\\x7F]'");
            syntax::terminal  &COLUMN   = terminal( ":",         ":", syntax::is_discardable );
            syntax::terminal  &END      = terminal( ";",         ";", syntax::is_discardable );
            syntax::terminal  &LPAREN   = terminal( "LPAREN",    "\\(", syntax::is_discardable );
            syntax::terminal  &RPAREN   = terminal( "RPAREN",    "\\)", syntax::is_discardable );
            syntax::terminal  &OPT      = terminal( "?", "\\?", syntax::is_specialized );
            syntax::terminal  &REP0     = terminal( "*", "\\*", syntax::is_specialized );
            syntax::terminal  &REP1     = terminal( "+", "\\+", syntax::is_specialized );
            syntax::terminal  &BAR      = terminal( "BAR",      "\\|", syntax::is_discardable);
            
            syntax::alternate &MODIF  = alt("MODIF");
            MODIF << OPT << REP0 << REP1;
            syntax::optional  &MODIFIER = opt("MODIFIER",MODIF);
            
            syntax::aggregate &RULE     = agg( "RULE" );
            //------------------------------------------------------------------
            // Rule prolog
            //------------------------------------------------------------------
            RULE << RULEID << COLUMN;
            
#if 0
            //------------------------------------------------------------------
            // Rule content
            //------------------------------------------------------------------
            syntax::alternate &ATOM     = alt("ATOM");
            syntax::aggregate &ITEM     = agg("ITEM", syntax::is_merging_one );
            syntax::repeating &ELEMENTS = rep("ELEMENTS", ITEM, 1);
            syntax::aggregate &GROUP     = agg("GROUP", syntax::is_merging_one);
            syntax::aggregate &ALT = agg("ALT");
            ALT << BAR << ELEMENTS;
            syntax::repeating &OTHER= rep("OTHER", ALT,0);
            GROUP << LPAREN << ELEMENTS << RPAREN;
            ATOM << RULEID << REGEXP << SINGLE << GROUP;
            ITEM << ATOM << MODIFIER << OTHER;
            
            RULE &= ELEMENTS;
#endif
            
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