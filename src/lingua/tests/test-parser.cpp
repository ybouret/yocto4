#include "yocto/lingua/parser.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/graphviz.hpp"

using namespace yocto;
using namespace lingua;

namespace
{

    class my_parser : public parser
    {
    public:
        explicit my_parser() : parser("my_parser","main")
        {

            Rule &ID  = terminal("ID","[:word:]+");
            Rule &INT = terminal("INT","[:digit:]+");
            Rule &EQ  = terminal("EQ",syntax::property::jettison);
            Rule &END = terminal("END",",",syntax::property::jettison);

            Alt  &VALUE = alt();
            VALUE << INT << ID;
            
            Agg  &ASSIGN = agg("ASSIGN");
            ASSIGN << ID << EQ << VALUE << END;

            Alt &STATEMENT = choice( ASSIGN, VALUE );

            top_level( zero_or_more(STATEMENT) );


            //__________________________________________________________________
            //
            // lexical rules
            //__________________________________________________________________
            root.endl("endl");
            root.drop("blank", "[:blank:]");

            graphviz("parser.dot");
            ios::graphviz_render("parser.dot");

        }

        virtual ~my_parser() throw()
        {
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(my_parser);
    };
    
}

#include "yocto/ios/icstream.hpp"

YOCTO_UNIT_TEST_IMPL(parser)
{
    my_parser     P;
    ios::icstream fp( ios::cstdin );
    
}
YOCTO_UNIT_TEST_DONE()
