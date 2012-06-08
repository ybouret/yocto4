#include "yocto/lang/parser.hpp"

namespace yocto 
{
    
    namespace lang 
    {
        
        parser:: ~parser() throw()
        {
        }
        
        
        parser:: parser( const string &lexer_main_id, const string &grammar_name ) :
        lexer(   lexer_main_id ),
        grammar( grammar_name  ),
        scan( first() )
        {
        }
        

        syntax::terminal &parser:: terminal( const string &id, const string &expr, syntax::node_property ppty   )
        {
            scan.make(id,expr);
            return term(id,ppty);
        }


    }

}
