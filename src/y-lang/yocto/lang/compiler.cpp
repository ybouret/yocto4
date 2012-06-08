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
            syntax::terminal &RULEID = terminal( "Rule Name", "[:word:]+" );
            syntax::terminal &REGEXP = terminal( "Reg. Exp.", "[:cstring:]");
            syntax::terminal &COLUMN = terminal( ":",         ":", syntax::is_discardable );
            syntax::terminal &END    = terminal( ";",         ";", syntax::is_discardable );
            
            syntax::aggregate &RULE  = agg( "RULE" );
            RULE << RULEID << COLUMN;
            
            RULE &= REGEXP;
            
            RULE &= END;
            
            
            syntax::repeating &GRAMMAR = rep("RULES",RULE,1);
            set_root(GRAMMAR);
        }
        
        
        syntax::parse_node * compiler:: operator()( ios::istream &fp )
        {
           
            regex::source       Source( fp );
            reset();
            syntax::parse_node *Tree = accept(*this, Source);
            
            return Tree;
        }

        
    }
    
}