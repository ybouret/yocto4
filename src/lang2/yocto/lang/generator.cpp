#include "yocto/lang/generator.hpp"

namespace yocto
{
    namespace lang
    {

        generator:: ~generator() throw()
        {
        }

        generator:: generator(const char *langID) :
        parser(langID,"main")
        {

            //__________________________________________________________________
            //
            // Terminal
            //__________________________________________________________________

            Rule &ID        = term("ID", "[:word:]+");
            Rule &COLON     = term(":",":",syntax::jettison);
            Rule &SEMICOLON = term(";",";",syntax::jettison);
            
            Rule &LPAREN    = term("(","\\(",syntax::univocal);
            Rule &RPAREN    = term(")","\\)",syntax::univocal);

            Agg  &RULE      = agg("RULE");

            RULE << ID << COLON;



            RULE << SEMICOLON;

            top_level( zero_or_more(RULE) );
            scanner.drop("WS", "[:blank:]");
            scanner.endl("ENDL");
        }

    }

}
