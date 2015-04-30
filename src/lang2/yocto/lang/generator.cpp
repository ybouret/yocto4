#include "yocto/lang/generator.hpp"
#include "yocto/lang/lexical/plugin/cstring.hpp"
#include "yocto/lang/lexical/plugin/comment.hpp"
#include "yocto/lang/lexical/plugin/ccomment.hpp"


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
            
            //Rule &LPAREN    = term("(","\\(",syntax::univocal);
            //Rule &RPAREN    = term(")","\\)",syntax::univocal);
            Rule &ALTERNATE = term("|","\\|",syntax::univocal);

            Agg  &RULE      = agg("RULE");

            RULE << ID << COLON;

            {
                Alt &ITEM = alt();
                ITEM << ID;
                ITEM << term<lexical::cstring>("regexp");

                Agg &ITEMS = agg("ITEMS");
                ITEMS << one_or_more(ITEM);
                Agg &EXTRA_ITEMS = agg("EXTRA_ITEMS");
                EXTRA_ITEMS << ALTERNATE << ITEMS;

                Agg &EXPR = agg("EXPR");
                EXPR << ITEMS << zero_or_more(EXTRA_ITEMS);

                RULE << EXPR;
            }

            RULE << SEMICOLON;

            top_level( zero_or_more(RULE) );

            load<lexical::comment>("C++ Comment","#").hook(scanner);
            load<lexical::ccomment>("C Comment","/\\*","\\*/").hook(scanner);

            scanner.drop("WS", "[:blank:]");
            scanner.endl("ENDL");
        }

    }

}
