#include "yocto/lingua/parser.hpp"

namespace yocto
{
    namespace lingua
    {

        parser:: ~parser() throw()
        {
            
        }
        
        parser:: parser( const string &title, const string &main_id) :
        lexer(   title + " Lexer"),
        grammar( title + " Grammar"),
        scanner( declare(main_id) )
        {
            
        }
        
        syntax::terminal & parser:: terminal( const string &id, const string &expr)
        {
            Y_LEX_FORWARD(scanner, id, expr);
            return term(id);
        }

        syntax::terminal & parser:: terminal( const char *id, const char *expr)
        {
            const string ID(id);
            const string EX(expr);
            return  terminal(ID, EX);
        }

        
    }

}

