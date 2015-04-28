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


    class Lexer : public lexer
    {
    public:
        virtual ~Lexer() throw() {}

        inline Lexer() : lexer("xLexer","root")
        {

            lexical::scanner &root = get_root();

            root.emit("ID",  "[_[:alpha:]][:word:]*");
            root.emit("INT", "[:digit:]+");
            root.emit(";", ";");

            lexical::plugin &cstring =load<lexical::cstring>("STR");
            cstring.hook(root);

            lexical::plugin &com     = load<lexical::comment>("COM","//");
            com.hook(root);

            root.drop("WS", "[:blank:]");
            root.endl("ENDL");
            root.emit("SNGL", ".");

        }



    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Lexer);
    };

    class Parser : public parser
    {
    public:
        explicit Parser() : parser("parser","root")
        {

            Rule &ID  = term("ID", "[_[:alpha:]][:word:]*");
            Rule &EQ  = term("=","=");
            Rule &INT = term("INT","[:digit]+");
            Rule &HEX = term("HEX", "0x[:xdigit:]+");
            Alt  &NUM = alt(); NUM << HEX << INT;
            Rule &END = term("END", ";");



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
