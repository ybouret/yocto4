#include "yocto/lang/generator.hpp"
#include "yocto/lang/lexical/plugin/cstring.hpp"
#include "yocto/lang/lexical/plugin/comment.hpp"
#include "yocto/lang/pattern/logic.hpp"

namespace yocto
{
    namespace lang
    {
        
        generator:: ~generator() throw()
        {
        }
        
        generator:: generator() :
        parser("generator","scanning")
        {
            //__________________________________________________________________
            //
            // Generator Grammar
            //__________________________________________________________________
            Terminal  &ID     = terminal("ID","[_[:alpha:]][:word:]*");
            Terminal  &COLON  = jettison("separator",':');
            Terminal  &STOP   = jettison("stop",';');
            Rule      &EXPR   = plug_term(new lexical::cstring("string",*this) );
            Terminal  &PIPE   = jettison("alternate",'|');
            Terminal  &CHAR   = terminal("char","'.'");
            Aggregate &RULE   = assemble("rule");
            
            RULE += ID;
            RULE += COLON;
            
            {
                Aggregate   & GROUP = agg( "group" );
                Alternate   & CORE  = alt();
                CORE |= ID;
                CORE |= EXPR;
                CORE |= CHAR;
                CORE |= GROUP;
                
                syntax::aggregate &ATOM = merge();
                ATOM += CORE;
                ATOM += opt( terminal("attr", "[+?*]"));
                
                syntax::aggregate &ATOMS    = merge();
                ATOMS += one_or_more(ATOM);
                ATOMS += zero_or_more(assemble("or",PIPE,ATOMS));
                
                GROUP += jettison("left paren",  '(');
                GROUP += ATOMS;
                GROUP += jettison("right paren", ')');
                
                RULE += ATOMS;
                
            }
            
            RULE += STOP;
            set_root( zero_or_more(RULE) );
            
            //__________________________________________________________________
            //
            // Lexical
            //__________________________________________________________________
            plug_meta( new lexical::comment("COMMENT",*this,"//") );
            scanner.make("blank", "[:blank:]", discard);
            scanner.make("endl" , "[:endl:]",  newline);
            
            
        }
        
        
        void generator:: rewrite(syntax::xnode *node) const throw()
        {
            if(node)
            {
            }
        }
        
    }
    
}


