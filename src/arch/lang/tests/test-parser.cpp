#include "yocto/lang/lexer.hpp"
#include "yocto/lang/syntax/xnode.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ptr/auto.hpp"

#include "yocto/lang/lexical/plugin/comment.hpp"
#include "yocto/lang/lexical/plugin/ccomment.hpp"
#include "yocto/lang/lexical/plugin/cstring.hpp"

#include "yocto/ios/icstream.hpp"
#include "yocto/lang/parser.hpp"

#include "yocto/fs/local-fs.hpp"
#include <cstdlib>


using namespace yocto;
using namespace lang;

namespace
{

    class JSON_Parser : public parser
    {
    public:

        explicit JSON_Parser() : parser("JSON", "main")
        {

            Rule &LBRACE = term("LBRACE","\\{", syntax::jettison);
            Rule &RBRACE = term("RBRACE","\\}", syntax::jettison);

            Rule &LBRACK = term("LBRACK","\\[", syntax::jettison);
            Rule &RBRACK = term("RBRACK","\\]", syntax::jettison);

            Rule &STRING = term<lexical::cstring>("String");
            Rule &COMA   = term("COMA",",", syntax::jettison);

            Alt  &JSON_Value  = alt();

            {
                JSON_Value
                << text("true",  syntax::univocal)
                << text("false", syntax::univocal)
                << term("Number","[:digit:]+")
                << STRING;
            }

            Alt &JSON_Object = alt();
            {
                {
                    Agg &EmptyObject = agg("EmptyObject");
                    EmptyObject << LBRACE << RBRACE;
                    JSON_Object << EmptyObject;
                }

                Agg &HeavyObject = agg("HeavyObject");
                HeavyObject << LBRACE;
                {
                    Agg &JSON_Pair = agg("Pair");
                    JSON_Pair   << STRING << term(":",":",syntax::jettison) << JSON_Value;
                    HeavyObject << JSON_Pair;
                    Agg &ExtraPair = agg("ExtraPair",syntax::mergeAll);
                    ExtraPair  << COMA << JSON_Pair;
                    HeavyObject << zero_or_more( ExtraPair );
                }
                HeavyObject << RBRACE;
                JSON_Object << HeavyObject;
            }

            JSON_Value << JSON_Object;

            Alt &JSON_Vector = alt();
            {
                {
                Agg &EmptyVector = agg("EmptyVector");
                EmptyVector << LBRACK << RBRACK;
                JSON_Vector << EmptyVector;
                }

                {
                    Agg &HeavyVector = agg("HeavyVector");
                    HeavyVector << LBRACK;
                    HeavyVector << JSON_Value;
                    Agg &ExtraValue = agg("ExtraValue",syntax::mergeAll);
                    ExtraValue << COMA << JSON_Value;
                    HeavyVector << zero_or_more(ExtraValue);
                    HeavyVector << RBRACK;
                    JSON_Vector << HeavyVector;
                }

            }

            JSON_Value << JSON_Vector;


            //__________________________________________________________________
            //
            // lexical
            //__________________________________________________________________
            scanner.drop("WS", "[:blank:]");
            scanner.endl("ENDL");

            top_level( choice(JSON_Object,JSON_Vector) );

            gramviz("gram.dot");
            (void) system("dot -Tpng -o gram.png gram.dot");
            finalize();
        }

        virtual ~JSON_Parser() throw()
        {
        }

    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(JSON_Parser);

    };

}

YOCTO_UNIT_TEST_IMPL(json)
{
    JSON_Parser P;
    P.show_followers();

    vfs &fs = local_fs::instance();
    fs.try_remove_file("xnode.dot");
    fs.try_remove_file("xnode.png");



    
    ios::icstream fp(ios::cstdin);
    auto_ptr<syntax::xnode> tree( P.run(fp) );
    if(tree.is_valid())
    {
        tree->graphviz("xnode.dot");
        (void) system("dot -Tpng -o xnode.png xnode.dot");
    }
    
}
YOCTO_UNIT_TEST_DONE()
