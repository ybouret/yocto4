#include "yocto/lang/generator.hpp"
#include "yocto/lang/lexical/plugin/cstring.hpp"
#include "yocto/lang/lexical/plugin/comment.hpp"

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
            Terminal  &ID     = terminal("ID","[_[:alpha:]][:word:]+");
            Terminal  &COLON  = jettison(":",':');
            Terminal  &END    = jettison(";",';');
            Rule      &STRING = plug_term(new lexical::cstring("STRING",*this) );
            Alternate &EXPR = alt();
            EXPR << ID << STRING;
            
            Aggregate &RULE = assemble("RULE");
            RULE << ID << COLON << EXPR << END;
            
            set_root( zero_or_more(RULE) );
            
            //__________________________________________________________________
            //
            // Lexical
            //__________________________________________________________________
            plug_meta( new lexical::comment("COMMENT",*this,"//") );
            scanner.make("blank", "[:blank:]", discard);
            scanner.make("endl" , "[:endl:]",  newline);

            
        }
        
    }
    
}


