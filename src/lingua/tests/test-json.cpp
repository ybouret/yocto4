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

            Alt &ARRAY = alt();
            {
                Rule &LBRACK = jettison('[');
                Rule &RBRACK = jettison(']');

                {
                    Agg &empty_array = agg("empty_array");
                    empty_array << LBRACK << RBRACK;
                    ARRAY |= empty_array;
                }

                {
                    Agg &heavy_array = agg("heavy_array");
                    heavy_array << LBRACK << VALUE << zero_or_more( agg("extra_value",property::temporary) << COMA << VALUE) << RBRACK;
                    ARRAY |= heavy_array;
                }

                VALUE << ARRAY;
            }

            Alt &OBJECT = alt();

            {
                Rule &LBRACE = jettison('{');
                Rule &RBRACE = jettison('}');

                {
                    Agg &empty_object = agg("empty_object");
                    empty_object << LBRACE << RBRACE;
                    OBJECT |= empty_object;
                }

                {
                    Agg &heavy_object = agg("heavy_object");
                    {
                        Agg &PAIR = agg("pair");
                        PAIR << STRING << jettison(':') << VALUE;
                        heavy_object << LBRACE << PAIR << zero_or_more( agg("extra_pair",property::temporary) << COMA << PAIR ) << RBRACE;
                    }
                    OBJECT |= heavy_object;
                }
                VALUE << OBJECT;
            }

            top_level( choice(OBJECT,ARRAY) );

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
    assert(tree.is_valid());
    tree->graphviz("tree.dot");
    ios::graphviz_render("tree.dot");
    
}
YOCTO_UNIT_TEST_DONE()
