#include "yocto/lang/compiler.hpp"

namespace yocto 
{
    namespace  lang 
    {
        
        compiler:: ~compiler() throw()
        {
            
        }
        
        void compiler:: enter_comment(const regex::token &) throw()
        {
        }
        
        void compiler:: leave_comment(const regex::token &) throw()
        {
            ++line;
        }
        
        compiler:: compiler() :
        parser( "lang.compiler.lexer", "lang.compiler.grammar" ),
        comment( declare("lang.compiler.comment") )
        {
            
            //------------------------------------------------------------------
            // scanner specifics
            //------------------------------------------------------------------
            scan.make( "BLANKS", "[ \t]+",    & scan.discard );
            scan.make( "ENDL"  , "[:endl:]+", & scan.no_endl );
            
            //------------------------------------------------------------------
            // declare terminals
            //------------------------------------------------------------------
            syntax::terminal  &RULEID   = terminal( "RULEID", "[:word:]+" );
            syntax::terminal  &REGEXP   = terminal( "REGEXP", "[:cstring:]");
            syntax::terminal  &SINGLE   = terminal( "SINGLE", "'[\\x20-\\x7F]'");
            syntax::terminal  &COLUMN   = terminal( ":",      ':', syntax::is_discardable );
            syntax::terminal  &END      = terminal( ";",      ';', syntax::is_discardable );
            syntax::terminal  &LPAREN   = terminal( "LPAREN", '(', syntax::is_discardable );
            syntax::terminal  &RPAREN   = terminal( "RPAREN", ')', syntax::is_discardable );
            syntax::terminal  &OPT      = terminal( "?",      '?', syntax::is_specialized );
            syntax::terminal  &REP0     = terminal( "*",      '*', syntax::is_specialized );
            syntax::terminal  &REP1     = terminal( "+",      '+', syntax::is_specialized );
            syntax::terminal  &BAR      = terminal( "BAR",    '|', syntax::is_discardable);
            
            //------------------------------------------------------------------
            // hand coded comment
            //------------------------------------------------------------------
            scan.call("lang.compiler.comment", "//", this, & compiler::enter_comment);
            
            comment.back("[:endl:]", this, & compiler::leave_comment );
            comment.make( "ANY1", ".", & comment.discard );
            
            syntax::alternate &MODIF  = alt("MODIF");
            MODIF << OPT << REP0 << REP1;
            syntax::optional  &MODIFIER = opt("MODIFIER",MODIF);
            
            syntax::aggregate &RULE     = agg( "RULE" );
            //------------------------------------------------------------------
            // Rule prolog
            //------------------------------------------------------------------
            RULE << RULEID << COLUMN;
            
            //------------------------------------------------------------------
            // Rule content
            //------------------------------------------------------------------
            syntax::alternate &ATOM     = alt("ATOM");
            syntax::aggregate &ITEM     = agg("ITEM", syntax::is_merging_one );
            syntax::repeating &ELEMENTS = rep("ELEMENTS", ITEM, 1);
            syntax::aggregate &GROUP    = agg("GROUP", syntax::is_merging_one);
            syntax::aggregate &ALT      = agg("ALT");
            
            syntax::terminal  &CODE       = terminal("CODE","@[[:word:][:digit:]]+");
            syntax::optional  &OPT_CODE  = opt("OPT_CODE",CODE);
            
            ALT << BAR << ELEMENTS;
            syntax::repeating &OTHER= rep("OTHER", ALT,0);
            GROUP << LPAREN << ELEMENTS << RPAREN;
            ATOM << RULEID << REGEXP << SINGLE << GROUP;
            ITEM << ATOM << MODIFIER << OPT_CODE << OTHER;
            
            RULE &= ELEMENTS;
            
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
        
        /**
         GRAMMAR : RULE+;
         RULE : RULEID ';';
         RULEID : "[:word:]+";
         REGEXP : "[:cstring:]";
         SINGLE : "'[\\x20-\\x7F]'"
         */
        
        
               
        
        syntax::parse_node * compiler:: operator()( ios::istream &fp )
        {
            regex::source  Source( fp );
            reset();
            syntax::parse_node *Tree = accept(*this, Source);
            
            //-- first pass: take care of specialized and discardable
            Tree->AST();
                                   
            return Tree;
        }
        
        
        
        
        
    }
    
}

