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

using namespace yocto;
using namespace lang;

namespace{


    class Parser : public parser
    {
    public:
        explicit Parser() : parser("parser","root")
        {

            Rule &ID = term("ID", "[_[:alpha:]][:word:]*");
            Rule &END = term("END",";");

            Agg &STATEMENT = agg("STATEMENT");
            STATEMENT << ID << END;

            top_level( zero_or_more(STATEMENT) );

            lexical::plugin &com     = load<lexical::comment>("COM","//");
            com.hook(scanner);
            scanner.drop("WS", "[:blank:]");
            scanner.endl("ENDL");
        }

        virtual ~Parser() throw() {}

    };

}

YOCTO_UNIT_TEST_IMPL(xnode)
{
    vfs &fs = local_fs::instance();
    fs.try_remove_file("xnode.dot");
    fs.try_remove_file("xnode.png");

    Parser P;

    ios::icstream fp(ios::cstdin);
    auto_ptr<syntax::xnode> tree( P.run(fp) );
    if(tree.is_valid())
    {
        tree->graphivz("xnode.dot");
        (void) system("dot -Tpng -o xnode.png xnode.dot");
    }
    
}
YOCTO_UNIT_TEST_DONE()
