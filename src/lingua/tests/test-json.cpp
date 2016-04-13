#include "yocto/lingua/parser.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/graphviz.hpp"
#include "yocto/lingua/lexical/plugin/cstring.hpp"

using namespace yocto;
using namespace lingua;

namespace
{
    class json_parser : public parser
    {
    public:
        explicit json_parser() : parser("JSON","root")
        {

            Alt  &VALUE  = alt();
            Rule &STRING = term<lexical::cstring>("string");
            Rule &COMA   = jettison(',');
            
            VALUE
            << univocal("null")
            << univocal("true")
            << univocal("false")
            << STRING
            << terminal("number","-?[:digit:]+");

            Rule &LBRACK = jettison('[');
            Rule &RBRACK = jettison(']');

            Alt &ARRAY = alt();
            {
                Agg &empty_array = agg("empty_array");
                empty_array << LBRACK << RBRACK;
                ARRAY |= empty_array;
            }

            {
                Agg &heavy_array = agg("heavy_array");
                heavy_array << LBRACK << VALUE << zero_or_more( agg("extra_value") << COMA << VALUE) << RBRACK;
                ARRAY |= heavy_array;
            }

            VALUE << ARRAY;

            top_level( zero_or_more(VALUE) );

            graphviz("json.dot");
            ios::graphviz_render("json.dot");

            //__________________________________________________________________
            //
            // lexical only rules
            //__________________________________________________________________
            root.drop("blank", "[:blank:]");
            root.endl("endl");
        }

        virtual ~json_parser() throw()
        {
        }

    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(json_parser);
    };
}

#include "yocto/ios/icstream.hpp"
#include "yocto/ptr/auto.hpp"

YOCTO_UNIT_TEST_IMPL(json)
{
    json_parser J;
    ios::icstream fp( ios::cstdin );
    auto_ptr<syntax::xnode> tree(J.parse(fp));
    if(tree.is_valid())
    {
        tree->graphviz("tree.dot");
        ios::graphviz_render("tree.dot");
    }

}
YOCTO_UNIT_TEST_DONE()
