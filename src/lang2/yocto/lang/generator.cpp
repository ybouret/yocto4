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
            
            Rule &LPAREN    = term("(","\\(",syntax::jettison);
            Rule &RPAREN    = term(")","\\)",syntax::jettison);
            Rule &ALTERNATE = term("|","\\|",syntax::univocal);

            Agg  &RULE      = agg("RULE");

            RULE << ID << COLON;

            {


                Alt &ATOM = alt();
                ATOM << ID;
                ATOM << term<lexical::cstring>("regexp");


                Agg  &ITEM  = agg("ITEM",syntax::mergeOne);
                ITEM << ATOM;
                Alt  &MOD  = alt();
                MOD << term("+","\\+",syntax::univocal);
                MOD << term("*","\\*",syntax::univocal);
                MOD << term("?","\\?",syntax::univocal);
                ITEM << opt(MOD);

                Rule &ITEMS = one_or_more(ITEM);
                Agg  &ALT   = agg("ALT");
                ALT << ALTERNATE << ITEMS;

                Agg  &SUB  = agg("SUB");
                SUB << ITEMS << zero_or_more(ALT);

                Agg &XPRN  = agg("XPRN", syntax::mergeOne);
                XPRN << LPAREN << SUB << RPAREN;

                ATOM << XPRN;

                RULE << SUB;
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
